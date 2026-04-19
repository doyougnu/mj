#ifndef J_AST_H
#define J_AST_H

#include "llvm/Support/SMLoc.h"
#include <cstdint>
#include <memory>
#include <stdfloat>
#include <string>
#include <variant>
#include <vector>

namespace j {

// TODO: move to a types module
using float64_t = double;

//===----------------------------------------------------------------------===//
// Forward declarations
//===----------------------------------------------------------------------===//

struct Expr;
using ExprPtr = std::unique_ptr<Expr>;

//===----------------------------------------------------------------------===//
// Data (Nouns) node
//===----------------------------------------------------------------------===//

struct IntLit {
  int64_t value;
};

struct FloatLit {
  float64_t value;
};

struct ArrayLit {
  std::vector<ExprPtr> elems;
};

struct Ident {
  std::string name;
};

//===----------------------------------------------------------------------===//
// Verb nodes
//===----------------------------------------------------------------------===//

enum class Prim {
  // Verbs
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
  // Adverbs
  Slash,
  Backslash,
  Tilde,
  // Conjunctions
  At,
  Amp,
  // Copulas
  GlobalAssign,
  LocalAssign
};

struct PrimVerb {
  Prim glyph;
};

struct AdverbApp {
  ExprPtr verb;
  Prim adverb;
};

struct ConjApp {
  ExprPtr lverb;
  Prim conj;
  ExprPtr rverb;
};

struct ForkApp {
  ExprPtr f, g, h;
};

struct AtopApp {
  ExprPtr f, g;
};

//===----------------------------------------------------------------------===//
// Application nodes
//===----------------------------------------------------------------------===//

struct MonadApp { // ala *:
  ExprPtr verb;
  ExprPtr rhs;
};

struct DyadApp { // ala 1 * 2
  ExprPtr verb;
  ExprPtr lhs;
  ExprPtr rhs;
};

//===----------------------------------------------------------------------===//
// Assignment
//===----------------------------------------------------------------------===//

enum class AssignKind { Local, Global };

struct Assign {
  std::string name;
  AssignKind kind;
  ExprPtr value;
};

//===----------------------------------------------------------------------===//
// Top level
//===----------------------------------------------------------------------===//

struct Module {
  std::vector<ExprPtr> sentences;
};

//===----------------------------------------------------------------------===//
// Expr
//===----------------------------------------------------------------------===//

using ExprKind = std::variant<IntLit,
                              FloatLit,
                              ArrayLit,
                              Ident,
                              PrimVerb,
                              AdverbApp,
                              ConjApp,
                              ForkApp,
                              AtopApp,
                              MonadApp,
                              DyadApp,
                              Assign>;

struct Expr {
  ExprKind kind;
  llvm::SMLoc loc;

  // convenience constructors
  // std::forward avoids copies here and propogates the val category
  template <typename T>
  Expr(T &&k, llvm::SMLoc l) : kind(std::forward<T>(k)), loc(l) {}

  template <class... Ts> struct cases : Ts... {
    using Ts::operator()...;
  };
};

//===----------------------------------------------------------------------===//
// Helpers for constructing nodes
//===----------------------------------------------------------------------===//

template <typename T, typename... Args>
ExprPtr make(llvm::SMLoc loc, Args &&...args) {
  return std::make_unique<Expr>(T{std::forward<Args>(args)...}, loc);
}

} // namespace j

#endif // J_AST_H
