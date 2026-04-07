#ifndef J_PRINTER_H
#define J_PRINTER_H

#include <llvm/Support/raw_os_ostream.h>
#include <variant>

#include "J/Ast.h"

using namespace j;

struct Printer {
  llvm::raw_ostream &os;
  int indent = 0;

  explicit Printer(llvm::raw_ostream &os = llvm::outs()) : os(os) {}

  // this is the injector to the printer
  void print(const ExprPtr &expr) { std::visit(*this, expr->kind); }

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
      print(elem);
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
    print(n.verb);
    --indent;
  }

  void operator()(const ConjApp &n) {
    printIndent();
    os << "ConjApp(" << primToString(n.conj) << ")\n";
    ++indent;
    print(n.lverb);
    print(n.rverb);
    --indent;
  }

  void operator()(const MonadApp &n) {
    printIndent();
    os << "MonadApp\n";
    ++indent;
    print(n.verb);
    print(n.rhs);
    --indent;
  }

  void operator()(const DyadApp &n) {
    printIndent();
    os << "DyadApp\n";
    ++indent;
    print(n.verb);
    print(n.lhs);
    print(n.rhs);
    --indent;
  }

  void operator()(const ForkApp &n) {
    printIndent();
    os << "ForkApp\n";
    ++indent;
    print(n.f);
    print(n.g);
    print(n.h);
    --indent;
  }

  void operator()(const AtopApp &n) {
    printIndent();
    os << "AtopApp\n";
    ++indent;
    print(n.f);
    print(n.g);
    --indent;
  }

  void operator()(const Assign &n) {
    printIndent();
    os << "Assign(" << n.name << ", "
       << (n.kind == AssignKind::Local ? "=." : "=:") << ")\n";
    ++indent;
    print(n.value);
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

#endif // J_PRINTER_H
