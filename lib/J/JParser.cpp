#include "J/JParser.h"
#include "J/JLexer.h"
#include "J/JOps.h"

#include "mlir/Dialect/Arith/IR/Arith.h"

using namespace j;

const JParser::PrattRule &JParser::getRule(Kind kind) {
  static const PrattRule table[] = {
      /* Token Kind          | Prefix (Nud)         | Infix (Led)          | BP
       */
      /* ------------------- | -------------------- | -------------------- | --
       */
      {Kind::Noun, &JParser::parseNoun, nullptr, 0},
      {Kind::EndOfFile, nullptr, nullptr, 0},

      /* Plus Family */
      {Kind::Plus, &JParser::parseMonad, &JParser::parseDyad, 10},      // +
      {Kind::PlusDot, &JParser::parseMonad, &JParser::parseDyad, 10},   // +.
      {Kind::PlusColon, &JParser::parseMonad, &JParser::parseDyad, 10}, // +:

      /* Assignment (Global and Local) */
      // {Kind::EqualDot, nullptr, &JParser::parseAssign, 5},   // =.
      // {Kind::EqualColon, nullptr, &JParser::parseAssign, 5}, // =:

      /* Adverbs (Bind tighter than verbs) */
      {Kind::Slash, nullptr, &JParser::parseAdv, 30},     // /
      {Kind::Backslash, nullptr, &JParser::parseAdv, 30}, // \

  };
  return table[static_cast<int>(kind)];
}

// In JParser.cpp
mlir::LogicalResult JParser::parse() {
  while (peek().kind != Token::EndOfFile) {
    // Start a new expression with 0 binding power
    auto result = parseExpr(0);

    if (!result)
      return mlir::failure();

    // Handle optional separators like Newline or Semicolon
    if (peek().kind == Token::Newline) {
      consume();
    }
  }
  return mlir::success();
}

mlir::Value JParser::parseExpr(int min_bp) {
  Token t = consume();
  auto nud_fn = getRule(t.kind).nud;
  if (!nud_fn)
    return nullptr;

  mlir::Value lhs = (this->*nud_fn)();

  while (min_bp < getRule(peek().kind).bp) {
    Token next = consume();
    auto led_fn = getRule(next.kind).led;
    lhs = (this->*led_fn)(lhs);
  }
  return lhs;
}

mlir::Value JParser::parseNoun() {
  // For now, let's just emit an arith.constant for a double
  double val;
  currentToken.text.getAsDouble(val);
  llvm::outs() << "CurrentTOk:" << currentToken.text << "\n";

  // TODO: handle if getAsDouble fails
  auto type = builder.getF64Type();
  return builder.create<mlir::arith::ConstantOp>(
      builder.getUnknownLoc(), type, builder.getF64FloatAttr(val));
}

mlir::Value JParser::parseMonad() {
  llvm::StringRef op = currentToken.text;
  llvm::outs() << "parse monad: " << currentToken.text << "\n";
  mlir::Value rhs = parseExpr(10); // Right-associative
  // TODO: more monads
  // return builder.create<j::NegateOp>(..., rhs);
  return rhs;
}

mlir::Value JParser::parseDyad(mlir::Value lhs) {
  llvm::StringRef op = currentToken.text;
  // J Verbs are right-associative, so we use the same BP (10)
  mlir::Value rhs = parseExpr(10);

  // TODO: dispatch the op
  return builder.create<j::PlusOp>(builder.getUnknownLoc(), lhs, rhs)
      ->getResult(0);
}

mlir::Value JParser::parseAdv(mlir::Value lhs) {
  // Adverbs (like /) modify the verb to their left
  // In MLIR, this might produce a "Rank-Reduced" operation
  return lhs;
}
