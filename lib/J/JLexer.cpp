#include "llvm/ADT/StringRef.h"

#include "J/JLexer.h"

#include <cctype>

using namespace j;

JLexer::JLexer(llvm::SourceMgr &sm) : sourceMgr(sm) {
        // Get a pointer to the start of the main buffer
        auto bufferID = sourceMgr.getMainFileID();
        curPtr = sourceMgr.getMemoryBuffer(bufferID)->getBufferStart();
    }

Token JLexer::getNextToken() {
   // If there is a peeked token waiting, take it and clear the cache
    if (lookahead.has_value()) {
        Token t = *lookahead;
        lookahead.reset();
        return t;
    }
    // Otherwise, just scan a fresh one
    return scanToken();
}

Token JLexer::scanToken() {
  // 1. Skip whitespace
  while (isspace(*curPtr)) ++curPtr;

  // 2. Check for End of File
  if (*curPtr == '\0') {
    return {Token::EOF_Token, "EOF", llvm::SMLoc::getFromPointer(curPtr)};
  }

  const char* startPtr = curPtr;
  llvm::SMLoc loc = llvm::SMLoc::getFromPointer(startPtr);

  // 3. Simple J-style logic (Nouns vs Verbs)
  if (isdigit(*curPtr)) {
    while (isdigit(*++curPtr)); // Consume numbers
    return {Token::Noun, llvm::StringRef(startPtr, curPtr - startPtr), loc};
  }

  if (ispunct(*curPtr)) {
    char op = *curPtr++;
    // In J, '+' is a verb, '/' might be an adverb
    if (op == '+' || op == '*' || op == '-') {
      return {Token::Verb, llvm::StringRef(startPtr, 1), loc};
    }
    if (op == '/') {
      return {Token::Adverb, llvm::StringRef(startPtr, 1), loc};
    }
  }

  return {Token::Error, "Unknown", loc};
}

Token JLexer::peek() {
  if (!lookahead.has_value()) {
    lookahead = scanToken();
  }
  return *lookahead;
}
