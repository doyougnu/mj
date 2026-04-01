#include "J/JDialect.h"
#include "J/JOps.h"
#include "mlir/IR/Builders.h"

using namespace mlir;
using namespace j;

// 1. Construct the Dialect
JDialect::JDialect(MLIRContext *context)
    : Dialect(getDialectNamespace(), context, TypeID::get<JDialect>()) {

  initialize();
}

// Optional: If you want to use standard MLIR types (like f64)
void JDialect::initialize() {
  addOperations< ConstantOp,
                PlusOp
                >();
}
