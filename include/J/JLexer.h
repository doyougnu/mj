#ifndef J_LEXER_H
#define J_LEXER_H

#include "llvm/Support/SourceMgr.h"
#include "llvm/ADT/StringRef.h"

namespace j {

struct Token {
  enum Kind { Noun, Verb, Adverb, EOF_Token, Error, Newline, Semicolon };
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

   llvm::SourceMgr &sourceMgr;
   std::optional<Token> lookahead;
   const char *curPtr;
};

}
#endif
