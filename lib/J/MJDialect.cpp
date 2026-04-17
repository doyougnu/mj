#include "J/MJDialect.h"
#include "J/JOps.h"

// Pull in the generated implementation logic
#define GET_DIALECT_DEFS
#include "J/MJDialect.cpp.inc"

#define GET_ATTRDEF_LIST
#include "J/MJAttrs.cpp.inc"

// void mj::MJDialect::initialize() {
// addOperations<mj::ConstantOp, mj::VerbOp, mj::DyadOpApp>();
// }

// NOTE: this is the "right way" as it automatically splices in the right stuff
// from the generated code. I'll keep it here for now and will use it in the
// future. But right now I want to see each op.
void mj::MJDialect::initialize() {
  addOperations<
#define GET_OP_LIST
#include "J/JOps.cpp.inc"
      >();
}
