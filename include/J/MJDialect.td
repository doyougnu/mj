#ifndef MJ_DIALECT
#define MJ_DIALECT

// We define a custom dialect in ODS that is then inherited by a hand written
// dialect in JDialect.h.
include "mlir/IR/OpBase.td"

def MJ_Base_Dialect : Dialect {
  let name = "mj";
  let cppNamespace = "mj";

  let summary = "The J language dialect.";

  // This tells MLIR that you will provide the implementation
  // of the initialize() function in your C++ file.
  let hasConstantMaterializer = 1;
  let useDefaultTypePrinterParser = 1;
  let useDefaultTypeID = 1;
}

#endif // MJ_DIALECT