#ifndef MJ_JOPS_H
#define MJ_JOPS_H

#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/Attributes.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"

#include "MJDialect.h"

// START: functions good enough add control flow and idents and functions for
// parser

// AttrClasses
#define GET_ATTRDEF_CLASSES
#include "J/JOps.h.inc"

// Pull in the Ops
#define GET_OP_CLASSES
#include "J/JOps.h.inc"

#endif // MJ_JOPS_H
