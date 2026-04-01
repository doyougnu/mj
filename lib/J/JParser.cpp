#include "J/JParser.h"
#include "J/JOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"

using namespace j;

JParser::JParser(mlir::MLIRContext *ctx, std::vector<Token> tokens)
    : tokens(tokens), builder(ctx) {}

const JParser::PrattRule& JParser::getRule(Kind kind) {
    static const PrattRule table[] = {
        /* Noun */   { &JParser::parseNoun,  nullptr,            0 },
        /* Verb */   { &JParser::parseMonad, &JParser::parseDyad, 10 },
        /* Adverb */ { nullptr,              &JParser::parseAdv,  30 },
        /* EOF */    { nullptr,              nullptr,            0 }
    };
    return table[static_cast<int>(kind)];
}

mlir::Value JParser::parseExpr(int min_bp) {
    Token t = consume();
    auto nud_fn = getRule(t.kind).nud;
    if (!nud_fn) return nullptr;

    mlir::Value lhs = (this->*nud_fn)();

    while (min_bp < getRule(peek().kind).bp) {
        t = consume();
        auto led_fn = getRule(t.kind).led;
        lhs = (this->*led_fn)(lhs);
    }
    return lhs;
}

// --- Implementation of Nuds and Leds ---

mlir::Value JParser::parseNoun() {
    // For now, let's just emit an arith.constant for a double
    double val = std::stod(last_token.text);
    auto type = builder.getF64Type();
    return builder.create<mlir::arith::ConstantOp>(
        builder.getUnknownLoc(), type, builder.getF64FloatAttr(val));
}

mlir::Value JParser::parseMonad() {
    std::string op = last_token.text;
    mlir::Value rhs = parseExpr(10); // Right-associative
    // return builder.create<j::NegateOp>(..., rhs);
    return rhs;
}

mlir::Value JParser::parseDyad(mlir::Value lhs) {
    std::string op = last_token.text;
    // J Verbs are right-associative, so we use the same BP (10)
    mlir::Value rhs = parseExpr(10);

    return builder.create<j::PlusOp>(builder.getUnknownLoc(), lhs, rhs)->getResult(0);
}

mlir::Value JParser::parseAdv(mlir::Value lhs) {
    // Adverbs (like /) modify the verb to their left
    // In MLIR, this might produce a "Rank-Reduced" operation
    return lhs;
}

// --- Helpers ---
Token JParser::consume() {
    if (pos < tokens.size()) return last_token = tokens[pos++];
    return {Kind::EOF_TK, ""};
}

Token JParser::peek() {
    if (pos < tokens.size()) return tokens[pos];
    return {Kind::EOF_TK, ""};
}
