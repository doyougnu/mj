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
    // control Flow if
    If,
    Do,
    Else,
    End,
    // control flow while
    While,
    // special
    EndOfFile,
    Newline,
    Semicolon,
    GlobalAssign,
    LocalAssign,
    Colon,
    Dot,
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
  const llvm::SourceMgr &sourceMgr;

  inline bool isDone() const { return curPtr >= endPtr; };
  inline bool isControl(Token t) {
    switch (t.kind) {
    case Token::While:
    case Token::If:
    case Token::Do:
    case Token::Else:
    case Token::End:
      return true;
    default:
      return false;
    }
  }

  inline bool iscontrol(char c) {
    bool result = false;
    const char *start = curPtr;
    switch (c) {
    case 'w':
    case 'i':
    case 'd':
    case 'e': {
      while (isalpha(*++curPtr)) { // skip to next non-alpha
      }
      if (*curPtr == '.') { // each control flow ends with a .
        result = true;
      }
      curPtr = start;
      break;

    default:
      break;
    }
    }
    return result;
  }

private:
  Token scanToken();
  Token scanNumber();
  Token scanControl();
  Token scanPrimitive();
  Token scanIdentifier();
  Token scanWhitespace();
  Token::Kind dispatch(llvm::StringRef c) const;
  void skipWhitespace();

  std::optional<Token> lookahead;
};

} // namespace j
#endif
