#ifndef J_JPARSER_H
#define J_JPARSER_H

#include "J/Ast.h"
#include "J/JLexer.h"

#include "mlir/IR/Builders.h"

namespace j {

enum class WordClass { Noun, Verb, Adverb, Conj, LPrn, RPrn, End };

// A Token node pair, essentially a typing judgement of the node
struct Word {
  WordClass wc;
  ExprPtr node;

  // ownership constructor
  Word(WordClass wrd, ExprPtr &&pt) : wc(wrd), node(std::move(pt)) {};

  // default move ops
  Word(Word &&) = default;
  Word &operator=(Word &&) = default;

  // prevent Word w2 = w1
  Word(const Word &) = delete;
  Word &operator=(const Word &) = delete;
};

class JParser {
public:
  JParser(mlir::OpBuilder &b, j::JLexer &&l) : builder(b), lexer(l) {}

  // The entry point to get a single MLIR Value (the result of the J expr)
  std::optional<ExprPtr> parse();

private:
  mlir::OpBuilder &builder;
  j::JLexer &lexer;
  j::Token currentToken;

  // Parse functions
  std::optional<Word> parseSentence();
  std::optional<ExprPtr> parsePrimary(Token);

  // Helpers
  Token consume() {
    const Token t = lexer.getNextToken();
    currentToken = t;
    return t;
  };
  Token peek() const { return lexer.peek(); };
  WordClass classify(const Token &);
  bool isNumeric(const Token &);
  bool isVerb(const ExprPtr &);
};

} // namespace j

#endif
