#include "J/JParser.h"
#include "J/Ast.h"
#include "J/JLexer.h"
#include "J/JOps.h"

#include "mlir/Dialect/Arith/IR/Arith.h"
#include <cctype>
#include <cstdint>
#include <mlir/IR/Diagnostics.h>
#include <mlir/IR/Location.h>
#include <mlir/Support/LLVM.h>
#include <string>

using namespace j;

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
  case Token::GlobalAssign:
  case Token::LocalAssign:
    return WordClass::Copula;
  default:
    return WordClass::Verb;
  }
}

bool JParser::isNumeric(const Token &tok) {
  bool result = false;
  switch (tok.kind) {
  case Token::Int:
  case Token::Float: {
    result = true;
    break;
  }
  default:
    break;
  }
  return result;
}

bool JParser::isVerb(const ExprPtr &e) {
  if (!e)
    return false;
  return std::visit(Expr::cases{[](const PrimVerb &) { return true; },
                                [](const AdverbApp &) { return true; },
                                [](const ConjApp &) { return true; },
                                [](const ForkApp &) { return true; },
                                [](const AtopApp &) { return true; },
                                [](const auto &) { return false; }},
                    e->kind);
}

std::optional<ExprPtr> make_numeric(const Token &&tok) {
  switch (tok.kind) {
  case Token::Int:
    int64_t i_result;
    tok.text.getAsInteger(10, i_result);
    return j::make<IntLit>(tok.location, i_result);
  case Token::Float:
    float64_t f_result;
    tok.text.getAsDouble(f_result);
    return j::make<FloatLit>(tok.location, f_result);
  default:
    return std::nullopt;
  }
}

Prim tokenToPrim(Token::Kind k) {
  switch (k) {
  case Token::Plus:
    return Prim::Plus;
  case Token::Minus:
    return Prim::Minus;
  case Token::Star:
    return Prim::Star;
  case Token::Percent:
    return Prim::Percent;
  case Token::Caret:
    return Prim::Caret;
  case Token::Lt:
    return Prim::Lt;
  case Token::Gt:
    return Prim::Gt;
  case Token::Bar:
    return Prim::Bar;
  case Token::Hash:
    return Prim::Hash;
  case Token::Iota:
    return Prim::Iota;
  case Token::Eq:
    return Prim::Eq;
  case Token::Slash:
    return Prim::Slash;
  case Token::Backslash:
    return Prim::Backslash;
  case Token::Tilde:
    return Prim::Tilde;
  case Token::At:
    return Prim::At;
  case Token::Amp:
    return Prim::Amp;
  case Token::GlobalAssign:
    return Prim::GlobalAssign;
  case Token::LocalAssign:
    return Prim::LocalAssign;
  default:
    llvm_unreachable("not a primitive");
  }
}

std::optional<ExprPtr> JParser::parsePrimary(Token t) {
  llvm::SMLoc loc = t.location;

  switch (t.kind) {
  // ── nums ────────────────────────────────────────────────────
  case Token::Int:
  case Token::Float: {
    auto num = make_numeric(std::move(t)).value();
    std::vector<ExprPtr> elems;
    elems.push_back(std::move(num));

    // while the next token is numeric
    while (isNumeric(peek())) {
      Token next = consume();
      elems.push_back(make_numeric(std::move(next)).value());
    }

    if (elems.size() == 1)
      return std::move(elems[0]); // don't wrap singletons in a vec
    return j::make<ArrayLit>(loc, std::move(elems));
  }
  // ── prims ────────────────────────────────────────────────────
  case Token::Plus:
  case Token::Minus:
  case Token::Star:
  case Token::Percent:
  case Token::Caret:
  case Token::Lt:
  case Token::Gt:
  case Token::Bar:
  case Token::Hash:
  case Token::Iota:
  case Token::Eq: {
    return j::make<PrimVerb>(loc, tokenToPrim(t.kind));
  }
  // ── idents ────────────────────────────────────────────────────
  case Token::Ident:
    return j::make<Ident>(loc, t.text.str());

  // ── copulas ────────────────────────────────────────────────────
  case Token::GlobalAssign:
    return j::make<PrimVerb>(loc, tokenToPrim(t.kind));
  case Token::LocalAssign:
    return j::make<PrimVerb>(loc, tokenToPrim(t.kind));

  // ── adverbs ────────────────────────────────────────────────────
  case Token::Slash:
  case Token::Backslash:
  case Token::Tilde: {
    return j::make<PrimVerb>(loc, tokenToPrim(t.kind));
  }

  // ── conjunctions ───────────────────────────────────────────────
  case Token::At:
  case Token::Amp: {
    return j::make<PrimVerb>(loc, tokenToPrim(t.kind));
  }
    // ── parenthesized expressions ──────────────────────────────────
  case Token::LPrn: {
    auto inner = parseSentence();
    if (inner && consume().kind != Token::RPrn) {
      // TODO: investiget mlir and llvm error reporting
      // mlir::emitError(inner->node->loc, "parsePrimary: expecting right
      // paren");
      return std::nullopt; // error(loc, "expected closing ')'");
    }
    return std::move(inner->node);
  }

  // ── end of input ───────────────────────────────────────────────
  case Token::Newline:
  case Token::EndOfFile:
    return nullptr;

  default:
    lexer.sourceMgr.PrintMessage(loc,
                                 llvm::SourceMgr::DK_Error,
                                 "Parser.parsePrimary: unexpected token: " +
                                     t.text);
    return std::nullopt;
  }
}

std::optional<std::vector<ExprPtr>> JParser::parseManyUntil(Token::Kind k) {
  std::vector<ExprPtr> sentences{};

  while (currentToken.kind != k && !lexer.isDone()) {
    auto sentence = parseSentence();
    if (sentence) {
      sentences.push_back(std::move(sentence->node));
    }
  }
  return sentences;
}

// START: don't parse directly, use the stack, it is cleaner then we have the
// control flow toks as bookmarks
std::optional<ExprPtr> JParser::parseControl(Token::Kind k) {
  switch (k) {
  case Token::If: { // we are at the front of an If
    lexer.curPtr++; // move past the if tok
    auto condition = parseUntil(Token::Do).value().node;
    lexer.curPtr++; // skip Do
    auto thn = parseManyUntil(Token::Else).value();
    lexer.curPtr++; // skip else
    auto els = parseManyUntil(Token::End).value();
    const llvm::SMLoc loc = condition->loc;

    return make<IfNode>(
        loc, std::move(condition), std::move(thn), std::move(els));
  }
  case Token::Do:
  case Token::Else: {
    lexer.curPtr++; // skip the tok
    return std::nullopt;
  }
  case Token::While: // TODO
  default:
    return std::nullopt;
  }
}

std::optional<Word> JParser::parseSentence() {
  return parseUntil(Token::Newline);
}

std::optional<Word> JParser::parseUntil(Token::Kind tok_end) {
  std::vector<Word> stack;

  auto push = [&](Word w) { stack.push_back(std::move(w)); };
  auto top = [&](int i) -> Word & { return stack[stack.size() - 1 - i]; };
  auto popN = [&](int n) { stack.erase(stack.end() - n, stack.end()); };
  // auto current = [&]() -> Word & { return stack[stack.size() - 1]; };
  auto expect = [&](Token::Kind should_be) {
    return (peek().kind == should_be); // equality on kinds only
  };

  // try to reduce the top of the stack
  // returns true if a reduction was made
  auto reduce = [&]() -> bool {
    int n = stack.size();
    if (n < 2)
      return false;

    WordClass w0 = top(0).wc; // rightmost
    WordClass w1 = top(1).wc;
    WordClass w2 = n >= 3 ? top(2).wc : WordClass::End;
    // WordClass w3 = n >= 4 ? top(3).wc : WordClass::End;

    // V A → V  (adverb application: +/ -\ +~)
    if (w1 == WordClass::Verb && w0 == WordClass::Adverb) {
      auto verb = std::move(top(1).node);
      llvm::SMLoc loc = top(1).node->loc;
      auto adverb = std::get<PrimVerb>(top(0).node->kind).glyph;
      auto adv_app = make<j::AdverbApp>(loc, std::move(verb), adverb);
      popN(2);
      push(Word{WordClass::Verb, std::move(adv_app)});
      return true;
    }

    // V C V → V  (conjunction: f@g  f&g)
    if (w2 == WordClass::Verb && w1 == WordClass::Conj &&
        w0 == WordClass::Verb) {
      auto lv = std::move(top(2).node);
      auto conj = std::move(top(1).node);
      auto rv = std::move(top(0).node);
      llvm::SMLoc loc = conj->loc;
      Prim prm = std::get<PrimVerb>(conj->kind).glyph;
      auto conj_app = make<ConjApp>(loc, std::move(lv), prm, std::move(rv));
      popN(3);
      push(Word{WordClass::Verb, std::move(conj_app)});
      return true;
    }

    // N V N → N  (dyadic: 1 2 3 + 4 5 6)
    if (w2 == WordClass::Noun && w1 == WordClass::Verb &&
        w0 == WordClass::Noun) {
      auto lhs = std::move(top(2).node);
      auto verb = std::move(top(1).node);
      auto rhs = std::move(top(0).node);
      llvm::SMLoc loc = verb->loc;
      popN(3);
      push({WordClass::Noun,
            make<j::DyadApp>(
                loc, std::move(verb), std::move(lhs), std::move(rhs))});
      return true;
    }

    // V N → N  (monadic: + 1 2 3)
    if (w1 == WordClass::Verb && w0 == WordClass::Noun) {
      auto verb = std::move(top(1).node);
      auto rhs = std::move(top(0).node);
      llvm::SMLoc loc = verb->loc;
      auto mon_app = make<MonadApp>(loc, std::move(verb), std::move(rhs));
      popN(2);
      push({WordClass::Noun, std::move(mon_app)});
      return true;
    }

    // V V V → N  (fork: (+ - *))
    if (w2 == WordClass::Verb && w1 == WordClass::Verb &&
        w0 == WordClass::Verb) {
      auto f = std::move(top(2).node);
      auto g = std::move(top(1).node);
      auto h = std::move(top(0).node);
      llvm::SMLoc loc = g->loc;
      popN(3);
      auto frk = make<ForkApp>(loc, std::move(f), std::move(g), std::move(h));
      push({WordClass::Verb, std::move(frk)});
      return true;
    }

    // V V → V  (atop: f@:g, also hook: (+ *))
    if (w1 == WordClass::Verb && w0 == WordClass::Verb) {
      auto f = std::move(top(1).node);
      auto g = std::move(top(0).node);
      llvm::SMLoc loc = f->loc;
      auto atop_app = make<AtopApp>(loc, std::move(f), std::move(g));
      popN(2);
      push({WordClass::Verb, std::move(atop_app)});
      return true;
    }

    // Name Copula ?? -> Assignment we push an assignment node to the stack,
    // then pop that to the module sentences
    if (w2 == WordClass::Name && w1 == WordClass::Copula) {
      auto the_ident = std::move(top(2).node);
      auto the_copula = std::move(top(1).node);
      auto the_body = std::move(top(0).node); // 0 is rightmost so its the body
      llvm::SMLoc loc = the_copula->loc;
      popN(3);
      // TODO: dispatch local v global
      auto assign =
          make<Assign>(loc,
                       std::move(std::get<Ident>(the_ident->kind).name),
                       AssignKind::Global,
                       std::move(the_body));
      push({WordClass::Assignment, std::move(assign)});
      return true;
    }

    return false;
  };

  // shift all words onto the stack, reducing eagerly after each shift here is
  // the dataflow, we take tokens, parse them and throw them on the stack, this
  // means that the stack only holds word-classed exprs, and after a parse we no
  // longer have tokens, then we consume the stack until its a singleton and
  // thats the result.
  while (!lexer.isDone()) {
    Token t = consume();

    if (t.kind == Token::EndOfFile || t.kind == tok_end)
      break;

    // control flow
    if (lexer.isControl(t)) {
      auto control_node = parseControl(t.kind);
      if (control_node) {
        push({WordClass::Control, std::move(*control_node)});
      } // else we just have
    }
    if (t.kind == Token::LPrn) {
      consume();                    // throw away lPrn
      auto inner = parseSentence(); // recurse for parens
      if (expect(Token::RPrn)) {
        consume(); // throw away RPrn
      } // TODO: throw error
      WordClass wc =
          isVerb(inner.value().node) ? WordClass::Verb : WordClass::Noun;
      push({wc, std::move(inner.value().node)});
    } else {
      auto node = parsePrimary(t); // lex one token into a node

      // TODO: move name handler to a function
      // handle names
      WordClass wc = WordClass::Noun;
      if (isVerb(node.value()) && isCopula(node.value())) {
        wc = WordClass::Copula;
      } else if (isValidName(node.value())) {
        wc = WordClass::Name;
      } else { // just a verb
        wc = WordClass::Verb;
      }

      push({wc, std::move(node.value())});
    }

    // reduce as much as possible after each shift
    while (reduce()) {
    }
  }

  // final reductions
  while (reduce()) {
  }

  if (stack.size() != 1)
    return std::nullopt;

  return std::move(stack[0]);
}

std::optional<Module> JParser::parse() {
  Module mod{};
  while (!lexer.isDone()) {
    auto sentence = parseSentence();
    if (sentence) {
      mod.sentences.push_back(std::move(sentence->node));
    }
  }
  return mod;
}
