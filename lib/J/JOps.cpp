#include "mlir/IR/Builders.h"

#include "J/JOps.h"

// Generate the string conversion logic for your J glyphs
#define GET_ENUM_CLASS_DEFS
#include "J/JOps.cpp.inc"

#define GET_OP_CLASSES
#include "J/JOps.cpp.inc"
