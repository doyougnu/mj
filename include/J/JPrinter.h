#ifndef J_PRINTER_H
#define J_PRINTER_H

#include <llvm/Support/raw_os_ostream.h>
#include <variant>

#include "J/Ast.h"

namespace j {

struct JPrinter {
  llvm::raw_ostream &os;
  int indent = 0;

  explicit JPrinter(llvm::raw_ostream &os = llvm::outs()) : os(os) {}

  void print(const Module &mod) {
    for (const auto &sentence : mod.sentences) {
      printExpr(sentence);
    }
  }
  void printExpr(const ExprPtr &expr) { std::visit(*this, expr->kind); }

  void printIndent() {
    for (int i = 0; i < indent; ++i)
      os << "  ";
  }

  void operator()(const IntLit &n) {
    printIndent();
    os << "IntLit(" << n.value << ")\n";
  }

  void operator()(const FloatLit &n) {
    printIndent();
    os << "FloatLit(" << n.value << ")\n";
  }

  void operator()(const ArrayLit &n) {
    printIndent();
    os << "ArrayLit[\n";
    ++indent;
    for (const auto &elem : n.elems)
      printExpr(elem);
    --indent;
    printIndent();
    os << "]\n";
  }

  void operator()(const Ident &n) {
    printIndent();
    os << "Indent(" << n.name << ")\n";
  }

  void operator()(const PrimVerb &n) {
    printIndent();
    os << "PrimVerb(" << primToString(n.glyph) << ")\n";
  }

  void operator()(const AdverbApp &n) {
    printIndent();
    os << "AdverbApp(" << primToString(n.adverb) << ")\n";
    ++indent;
    printExpr(n.verb);
    --indent;
  }

  void operator()(const ConjApp &n) {
    printIndent();
    os << "ConjApp(" << primToString(n.conj) << ")\n";
    ++indent;
    printExpr(n.lverb);
    printExpr(n.rverb);
    --indent;
  }

  void operator()(const MonadApp &n) {
    printIndent();
    os << "MonadApp\n";
    ++indent;
    printExpr(n.verb);
    printExpr(n.rhs);
    --indent;
  }

  void operator()(const DyadApp &n) {
    printIndent();
    os << "DyadApp\n";
    ++indent;
    printExpr(n.verb);
    printExpr(n.lhs);
    printExpr(n.rhs);
    --indent;
  }

  void operator()(const ForkApp &n) {
    printIndent();
    os << "ForkApp\n";
    ++indent;
    printExpr(n.f);
    printExpr(n.g);
    printExpr(n.h);
    --indent;
  }

  void operator()(const AtopApp &n) {
    printIndent();
    os << "AtopApp\n";
    ++indent;
    printExpr(n.f);
    printExpr(n.g);
    --indent;
  }

  void operator()(const IfNode &iff) {
    printIndent();
    os << "IfNode\n";
    ++indent;
    os << "Condition:";
    printExpr(iff.cond);

    os << "Then:\n";
    for (auto &t : iff.thn) {
      printExpr(t);
    }

    os << "Else:\n";
    for (auto &e : iff.els) {
      printExpr(e);
    }
  }

  void operator()(const Assign &n) {
    printIndent();
    os << "Assign(" << n.name << ", "
       << (n.kind == AssignKind::Local ? "=." : "=:") << ")\n";
    ++indent;
    printExpr(n.value);
    --indent;
  }

private:
  llvm::StringRef primToString(Prim p) {
    switch (p) {
    case Prim::Plus:
      return "+";
    case Prim::Minus:
      return "-";
    case Prim::Star:
      return "*";
    case Prim::Percent:
      return "%";
    case Prim::Caret:
      return "^";
    case Prim::Lt:
      return "<";
    case Prim::Gt:
      return ">";
    case Prim::Bar:
      return "|";
    case Prim::Hash:
      return "#";
    case Prim::Iota:
      return "i.";
    case Prim::Eq:
      return "=";
    case Prim::GlobalAssign:
      return "=:";
    case Prim::LocalAssign:
      return "=.";
    case Prim::Slash:
      return "/";
    case Prim::Backslash:
      return "\\";
    case Prim::Tilde:
      return "~";
    case Prim::At:
      return "@";
    case Prim::Amp:
      return "&";
    }
  }
};
} // end namespace j
#endif // J_PRINTER_H
