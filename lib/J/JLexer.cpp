#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"

#include "llvm/Support/raw_ostream.h"

#include "J/JLexer.h"

#include <cctype>

using namespace j;

JLexer::JLexer(llvm::SourceMgr &sm) : sourceMgr(sm) {
  // Get a pointer to the start of the main buffer
  auto bufferID = sourceMgr.getMainFileID();
  curPtr = sourceMgr.getMemoryBuffer(bufferID)->getBufferStart();
  endPtr = sourceMgr.getMemoryBuffer(bufferID)->getBufferEnd();
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

void JLexer::skipWhitespace() {
  while (isspace(*curPtr)) {
    ++curPtr;
  }
  return;
}

Token JLexer::scanNumber() {
  const char *start = curPtr;
  llvm::SMLoc loc = llvm::SMLoc::getFromPointer(start);
  while (isdigit(*++curPtr))
    ;
  // TODO: implement floats
  return {Token::Int, llvm::StringRef(start, curPtr - start), loc};
}

Token JLexer::scanControl() {
  const char *start = curPtr;
  llvm::SMLoc loc = llvm::SMLoc::getFromPointer(start);

  while (isalpha(*++curPtr)) { // get to the .
  }
  curPtr++; // consume the .
  llvm::StringRef op = llvm::StringRef(start, curPtr - start);
  const Token::Kind tok = dispatch(op);
  return {tok, llvm::StringRef(start, curPtr - start), loc};
}

Token JLexer::scanPrimitive() {
  const char *start = curPtr;
  const llvm::SMLoc loc = llvm::SMLoc::getFromPointer(start);
  // TODO: why wasn't this used? const char base = *curPtr++;
  curPtr++;

  if (*curPtr == '.' || *curPtr == ':') {
    curPtr++; // move pointer to include it
  }

  llvm::StringRef op = llvm::StringRef(start, curPtr - start);
  const Token::Kind tok = dispatch(op);
  return {tok, llvm::StringRef(start, curPtr - start), loc};
}

Token JLexer::scanIdentifier() {
  const char *start = curPtr;
  while (isalpha(*++curPtr))
    ;
  const llvm::SMLoc loc = llvm::SMLoc::getFromPointer(start);
  return {Token::Ident, llvm::StringRef(start, curPtr - start), loc};
}

Token JLexer::scanToken() {
  llvm::SMLoc loc = llvm::SMLoc::getFromPointer(curPtr);
  Token result = {Token::Error, "Lexer: Something bad", loc};

  skipWhitespace();
  if (curPtr >= endPtr)
    return {Token::Kind::EndOfFile, "", loc};

  char c = *curPtr;

  if (iscontrol(c))
    result = scanControl();

  if (isdigit(c)) // TODO: negatives || (c == '_' && isdigit(peekNext())))
    result = scanNumber();

  if (ispunct(c))
    result = scanPrimitive();

  if (isalpha(c))
    result = scanIdentifier();
  return result;
}

Token::Kind JLexer::dispatch(llvm::StringRef c) const {
  return llvm::StringSwitch<Token::Kind>(c)
      // inflections first, order is matched top down
      .StartsWith("=:", Token::GlobalAssign)
      .StartsWith("=.", Token::LocalAssign)
      .StartsWith("if.", Token::If)
      .StartsWith("do.", Token::Do)
      .StartsWith("else.", Token::Else)
      .StartsWith("end.", Token::End)
      .StartsWith("i.", Token::Iota)
      .StartsWith("+", Token::Plus)
      .StartsWith("_", Token::Minus)
      .StartsWith("*", Token::Star)
      .StartsWith("%", Token::Percent)
      .StartsWith("^", Token::Caret)
      .StartsWith("<", Token::Lt)
      .StartsWith(">", Token::Gt)
      .StartsWith("=", Token::Eq)
      .StartsWith("-", Token::Bar)
      .StartsWith("#", Token::Hash)
      .StartsWith("~", Token::Tilde)
      .StartsWith("@", Token::At)
      .StartsWith("&", Token::Amp)
      .StartsWith("(", Token::LPrn)
      .StartsWith(")", Token::RPrn)
      .StartsWith("\\", Token::Backslash)
      .StartsWith("\n", Token::Newline)
      .StartsWith("/", Token::Slash)
      .StartsWith(".", Token::Dot)
      .StartsWith(";", Token::Semicolon)
      .StartsWith(":", Token::Colon);
}

Token JLexer::peek() {
  if (!lookahead.has_value()) {
    lookahead = scanToken();
  }
  return *lookahead;
}
