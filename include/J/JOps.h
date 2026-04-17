#ifndef MJ_JOPS_H
#define MJ_JOPS_H

#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#include "MJDialect.h"

// START: namespace issues are now a real problem. Time to clean!
// THEN: add control flow and idents and functions for parser

// AttrClasses
#define GET_ATTRDEF_CLASSES
#include "JOps.h.inc"

// Pull in the Ops
#define GET_OP_CLASSES
#include "JOps.h.inc"

#endif // MJ_JOPS_H
