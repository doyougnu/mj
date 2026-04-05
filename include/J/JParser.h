#ifndef J_JPARSER_H
#define J_JPARSER_H

#include "J/JLexer.h"

#include "mlir/IR/Builders.h"
#include "mlir/IR/Value.h"

#include <memory>

// START: write AST.h, remove pratt for a push-reduce parser

namespace j {

enum class WordClass { Noun, Verb, Adverb, Conj, LPrn, RPrn, End };

struct Node {
  virtual ~Node() = default;
  virtual bool isVerb() const { return false; }
  llvm::SMLoc pos;
};

using NodePtr = std::unique_ptr<Node>;

struct Word {
  WordClass wc;
  NodePtr node;
};

WordClass classify(const j::Token &tok) {
  switch (tok.kind) {
  case Token::Int:
  case Token::Float:
  case Token::Ident:
    return WordClass::Noun;
  case Token::Slash:
  case Token::Backslash:
  case Token::Tilde:
    return WordClass::Adverb;
  case Token::At:
  case Token::Amp:
    return WordClass::Conj;
  case Token::LPrn:
    return WordClass::LPrn;
  case Token::RPrn:
    return WordClass::RPrn;
  default:
    return WordClass::Verb;
  }

  class JParser {
  public:
    JParser(mlir::OpBuilder &b, j::JLexer &l)
        : builder(b), lexer(std::move(l)) {
      currentToken = lexer.getNextToken();
      // last_token = currentToken;
    }

    // The entry point to get a single MLIR Value (the result of the J expr)
    mlir::LogicalResult parse();

  private:
    struct PrattRule {
      // Nud is a token with nothing to the left
      using NudFn = mlir::Value (JParser::*)();
      // Led is a left dominator, means the token needs a value from its left
      using LedFn = mlir::Value (JParser::*)(mlir::Value);
      Token::Kind kind;
      NudFn nud;
      LedFn led;
      int bp;
    };

    mlir::OpBuilder &builder;
    j::JLexer &&lexer;

    j::Token currentToken;
    // j::Token last_token;

    using Kind = j::Token::Kind;
    static const PrattRule &getRule(Kind kind);

    // Parse functions
    mlir::Value parseExpr(int min_bp = 0);
    mlir::Value parseNoun();                // Nud: handles numbers/arrays
    mlir::Value parseMonad();               // Nud: handles -y
    mlir::Value parseDyad(mlir::Value lhs); // Led: handles x + y
    mlir::Value parseAdv(mlir::Value lhs);  // Led: handles +/

    // Helpers
    Token consume() {
      const Token t = lexer.getNextToken();
      currentToken = t;
      return t;
    };
    Token peek() const { return lexer.peek(); };
  };

} // namespace j

#endif
