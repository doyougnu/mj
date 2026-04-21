#ifndef J_JPARSER_H
#define J_JPARSER_H

#include "J/Ast.h"
#include "J/JLexer.h"

#include "mlir/IR/Builders.h"

namespace j {

enum class WordClass {
  Noun,
  Verb,
  Adverb,
  Conj,
  Copula,
  Name,
  LPrn,
  RPrn,
  Assignment,
  Control,
  End
};

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
  JParser(mlir::OpBuilder &b, j::JLexer &&l) : lexer(l) {}

  // The entry point to get a single MLIR Value (the result of the J expr)
  std::optional<Module> parse();

private:
  j::JLexer &lexer;
  j::Token currentToken;

  // Parse functions
  std::optional<Word> parseSentence();
  std::optional<ExprPtr> parsePrimary(Token);
  std::optional<Word> parseUntil(Token::Kind);
  std::optional<std::vector<ExprPtr>> parseManyUntil(Token::Kind);
  std::optional<ExprPtr> parseControl(Token::Kind);

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
  bool isCopula(const ExprPtr &e) {
    return (std::get<PrimVerb>(e->kind).glyph == Prim::GlobalAssign) ||
           (std::get<PrimVerb>(e->kind).glyph == Prim::LocalAssign);
  }
  // TODO: move to Name newtype, parse, don't validate
  bool isValidName(const ExprPtr &e) {
    std::string s{};
    if (const Ident *st = std::get_if<Ident>(&e->kind)) {
      s = st->name;
    }
    if (s.empty() || !std::isalpha(static_cast<unsigned char>(s[0])))
      return false;

    for (unsigned char c : s) {
      // J names allow alpha, digits, and underscores
      if (!std::isalnum(c) && c != '_') {
        return false;
      }
    }
    return true;
  }
};

} // namespace j

#endif
