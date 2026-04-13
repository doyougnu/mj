#ifndef J_JDIALECT_H
#define J_JDIALECT_H

#include "mlir/IR/Dialect.h"

namespace j {
class JDialect : public mlir::Dialect {

  // START: tblgen ops, then JAstToMlir sinks to mj dialect, then rewrite over
  // that dialect
public:
  explicit JDialect(mlir::MLIRContext *ctx);
  static llvm::StringRef getDialectNamespace() { return "mj"; }

  void initialize();
};
} // namespace j

#endif
