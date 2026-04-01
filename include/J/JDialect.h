#ifndef J_JDIALECT_H
#define J_JDIALECT_H

#include "mlir/IR/Dialect.h"

namespace j {
class JDialect : public mlir::Dialect {
public:
  explicit JDialect(mlir::MLIRContext *ctx);
  static llvm::StringRef getDialectNamespace() { return "j"; }
};
} // namespace j

#endif
