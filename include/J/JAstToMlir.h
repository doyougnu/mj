#ifndef J_ASTTOMLIR_H
#define J_ASTTOMLIR_H

#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/IR/Diagnostics.h>

#include "J/Ast.h"

namespace j {

class JAstToMlir {

public:
  mlir::OpBuilder builder;
  const llvm::SourceMgr &srcMgr;

  explicit JAstToMlir(mlir::OpBuilder b, llvm::SourceMgr &s)
      : builder(b), srcMgr(s) {};
  mlir::Value sink(const ExprPtr &expr);

private:
  mlir::Location mk_loc(llvm::SMLoc loc);

  mlir::Value operator()(const IntLit &n, mlir::Location loc);

  mlir::Value operator()(const FloatLit &n, mlir::Location loc);

  mlir::Value operator()(const ArrayLit &n, mlir::Location loc);

  mlir::Value operator()(const Ident &n, mlir::Location loc);

  mlir::Value operator()(const PrimVerb &n, mlir::Location loc);

  mlir::Value operator()(const AdverbApp &n, mlir::Location loc);

  mlir::Value operator()(const ConjApp &n, mlir::Location loc);

  mlir::Value operator()(const MonadApp &n, mlir::Location loc);

  mlir::Value operator()(const DyadApp &n, mlir::Location loc);

  mlir::Value operator()(const ForkApp &n, mlir::Location loc);

  mlir::Value operator()(const AtopApp &n, mlir::Location loc);

  mlir::Value operator()(const Assign &n, mlir::Location loc);
};

} // end namespace j
#endif // J_ASTTOMLIR_H
