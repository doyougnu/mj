#ifndef MJ_DIALECT_H
#define MJ_DIALECT_H

// must come before the generated inc files
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Support/TypeID.h"

#include <compare>

#define GET_ENUM_CLASSES
#define GET_ATTRDEF_CLASSES
#include "J/MJAttrs.h.inc"

#include "J/MJDialect.h.inc"

#endif
