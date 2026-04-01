#include "J/JOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/OpImplementation.h"

using namespace j;

/**
 * The build method is called by your JParser.
 * It populates the OperationState which MLIR uses to create the actual Op.
 */
void PlusOp::build(mlir::OpBuilder &builder, mlir::OperationState &state,
                   mlir::Value lhs, mlir::Value rhs) {
    // 1. Add the operands (x and y in J's x + y)
    state.addOperands({lhs, rhs});

    // 2. Infer the result type.
    // In a "Pure C++" Dialect, you decide the logic.
    // For now, we assume the result type is the same as the LHS.
    // Real J logic would check ranks and promote types (e.g., int to float).
    state.addTypes(lhs.getType());
}

/**
 * The verify method is called by the MLIR Verifier pass.
 * This is where you enforce J-specific constraints.
 */
mlir::LogicalResult PlusOp::verify() {
    auto lhsType = getLHS().getType();
    auto rhsType = getRHS().getType();

    // Basic check: Ensure both operands are some form of Tensor/Array
    if (lhsType != rhsType) {
        return emitOpError("requires operands to have the same type (for now)");
    }

    return mlir::success();
}
