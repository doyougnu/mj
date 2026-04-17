#include "J/MJDialect.h"
#include "J/JOps.h"

// Pull in the generated implementation logic
#define GET_DIALECT_DEFS
#include "MJDialect.cpp.inc"

#define GET_ATTRDEF_LIST
#include "MJAttrs.cpp.inc"

// using namespace mlir;
// using namespace mj;

// 1. Construct the Dialect
// MJDialect::MJDialect(MLIRContext *context)
// : Dialect(getDialectNamespace(), context, TypeID::get<MJDialect>()) {

// initialize();
// }

// Optional: If you want to use standard MLIR types (like f64)
// void MJDialect::initialize() { addOperations<j::ConstantOp, j::PlusOp>(); }

// NOTE: this is the "right way" as it automatically splices in the right stuff
// from the generated code. I'll keep it here for now and will use it in the
// future. But right now I want to see each op.
// void MJDialect::initialize() {
// addOperations<
// #define GET_OP_LIST
// #include "mj/JOps.cpp.inc"
// >();
// }
