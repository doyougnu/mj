#include "llvm/Support/SourceMgr.h"

#include <mlir/IR/Location.h>
#include <variant>

#include <J/Ast.h>
#include <J/JAstToMlir.h>

//===----------------------------------------------------------------------===//
// Helpers
//===----------------------------------------------------------------------===//
mlir::Location j::JAstToMlir::mk_loc(llvm::SMLoc loc) {

  auto lineCol = srcMgr.getLineAndColumn(loc);
  auto bufferId = srcMgr.getBufferInfo(srcMgr.FindBufferContainingLoc(loc))
                      .Buffer->getBufferIdentifier();

  return mlir::FileLineColLoc::get(
      builder.getStringAttr(bufferId), lineCol.first, lineCol.second);
}

//===----------------------------------------------------------------------===//
// Overloads
//===----------------------------------------------------------------------===//
mlir::Value j::JAstToMlir::operator()(const IntLit &n, mlir::Location loc) {
  auto type = builder.getI64Type();
  return builder.create<mlir::arith::ConstantOp>(
      loc, type, builder.getIntegerAttr(type, n.value));
}

mlir::Value j::JAstToMlir::operator()(const FloatLit &n, mlir::Location loc) {
  auto type = builder.getF64Type();
  return builder.create<mlir::arith::ConstantOp>(
      loc, type, builder.getFloatAttr(type, n.value));
}

mlir::Value j::JAstToMlir::operator()(const ArrayLit &n, mlir::Location loc) {

  // Collect the raw values from your AST nodes
  // Note: This assumes a flat array for now.
  // J's multi-dimensional arrays would require checking n.shape.
  std::vector<int64_t> rawValues;
  for (const auto &expr : n.elems) {
    // We expect these to be IntLits for an integer array constant
    // TODO: allow parametric polymorphic arrays
    if (auto *intNode = std::get_if<IntLit>(&expr->kind)) {
      rawValues.push_back(intNode->value);
    }
  }

  // Define the MLIR Shape and Type
  // For J vector '1 2 3', shape is [3]
  int64_t shape[] = {static_cast<int64_t>(rawValues.size())};
  auto elementType = builder.getI64Type();
  auto tensorType = mlir::RankedTensorType::get(shape, elementType);

  auto dataAttr =
      mlir::DenseElementsAttr::get(tensorType, llvm::ArrayRef(rawValues));

  return builder.create<mlir::arith::ConstantOp>(loc, tensorType, dataAttr);
}

mlir::Value j::JAstToMlir::operator()(const Ident &s, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const PrimVerb &pv, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const AdverbApp &aa, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const ConjApp &ca, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const MonadApp &a, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const j::DyadApp &n, mlir::Location loc) {
  mlir::Value lhs = sink(n.lhs);
  mlir::Value rhs = sink(n.rhs);
  if (!lhs || !rhs)
    return nullptr;

  // Dispatch on the verb field (ExprPtr)
  if (auto *prim = std::get_if<j::PrimVerb>(&n.verb->kind)) {

    // 3. Match the specific J character
    if (prim->glyph == Prim::Plus) {
      // TODO: type check before lowering? auto operands = matchTypes(lhs, rhs,
      // loc);
      // TODO: lower to the j dialect, after installing the dyad op
      return builder.create<mlir::arith::AddIOp>(loc, lhs, rhs);
    }
    if (prim->glyph == Prim::Minus) {
      return builder.create<mlir::arith::SubIOp>(loc, lhs, rhs);
    }
    // ... other primitives ...
  }

  // If the verb isn't a PrimVerb (e.g., it's a Gerund or a Name),
  // TODO: fill out

  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const ForkApp &fa, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const AtopApp &aa, mlir::Location loc) {
  return nullptr;
}

mlir::Value j::JAstToMlir::operator()(const Assign &a, mlir::Location loc) {
  return nullptr;
}

//===----------------------------------------------------------------------===//
// Api
//===----------------------------------------------------------------------===//
mlir::Value j::JAstToMlir::sink(const ExprPtr &expr) {
  if (!expr)
    return nullptr;
  return std::visit(
      [&](const auto &node) -> mlir::Value {
        return (*this)(node, mk_loc(expr->loc));
      },
      expr->kind);
}
