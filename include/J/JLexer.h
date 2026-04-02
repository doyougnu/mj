#ifndef J_LEXER_H
#define J_LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/SourceMgr.h"

namespace j {

struct Token {
  enum Kind {
    Noun,
    EndOfFile,
    Plus,
    PlusDot,
    PlusColon,
    EqualDot,
    EqualColon,
    Slash,
    Backslash,
    Newline,
    Semicolon,
    Error,
    Ident
  };
  Kind kind;
  llvm::StringRef text;
  llvm::SMLoc location;
};

class JLexer {
public:
  JLexer(llvm::SourceMgr &sm);

  Token getNextToken();
  Token peek();

private:
  Token scanToken();
  Token scanNumber();
  Token scanPrimitive();
  Token scanIdentifier();
  Token::Kind dispatch(llvm::StringRef c) const;
  void skipWhitespace();

  llvm::SourceMgr &sourceMgr;
  std::optional<Token> lookahead;
  const char *curPtr;
  const char *endPtr;
};

} // namespace j
#endif
