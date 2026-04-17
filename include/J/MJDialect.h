#ifndef MJ_DIALECT_H
#define MJ_DIALECT_H

// must come before the generated inc files
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/Support/TypeID.h"

#include <compare>

namespace mj {

#define GET_ENUM_CLASSES
#include "MJAttrs.h.inc"

#define GET_ATTRDEF_CLASSES
#include "MJAttrs.h.inc"

// dialect class first
#include "MJDialect.h.inc"

} // namespace mj

#endif
