#ifndef J_LEXER_H
#define J_LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SourceMgr.h"

namespace j {

struct Token {
  enum Kind {
    // nums
    Int,
    Float,
    // prims
    Plus,
    Minus,
    Star,
    Percent,
    Caret,
    Lt,
    Gt,
    Bar,
    Hash,
    Iota,
    Eq,
    // idents
    Ident,
    // adverbs
    Tilde,
    Slash,
    Backslash,
    // conjunctions
    At,
    Amp,
    LPrn,
    RPrn,
    // special
    EndOfFile,
    Newline,
    Semicolon,
    Error,

  };

  Kind kind;
  llvm::StringRef text;
  llvm::SMLoc location;
};

class JLexer {
public:
  explicit JLexer(llvm::SourceMgr &sm);

  Token getNextToken();
  Token peek();
  const char *curPtr;
  const char *endPtr;

private:
  Token scanToken();
  Token scanNumber();
  Token scanPrimitive();
  Token scanIdentifier();
  Token::Kind dispatch(llvm::StringRef c) const;
  void skipWhitespace();

  const llvm::SourceMgr &sourceMgr;
  std::optional<Token> lookahead;
};

} // namespace j
#endif
