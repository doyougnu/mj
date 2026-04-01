#include "J/JOps.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/OpImplementation.h"

using namespace j;

/*************************** Constants ****************************************/
void ConstantOp::build(mlir::OpBuilder &builder, mlir::OperationState &state,
                       mlir::DenseElementsAttr value) {
  state.addAttribute("value", value);
  state.addTypes(value.getType());
}

void ConstantOp::build(mlir::OpBuilder &builder, mlir::OperationState &state,
                       mlir::Type result, mlir::DenseElementsAttr value) {
  // 1. Add the "value" attribute.
  // This stores the actual array data (e.g., [1, 2, 3]).
  state.addAttribute("value", value);

  // 2. Set the result type.
  // In J, this would typically be a RankedTensorType
  // representing the shape and element type (e.g., tensor<3xf64>).
  state.addTypes(result);
}

void ConstantOp::build(mlir::OpBuilder &builder, mlir::OperationState &state,
                       double value) {
 // 1. Create a Float Attribute to store the actual double value
    auto dataType = builder.getF64Type();
    auto dataAttr = builder.getFloatAttr(dataType, value);

    // 2. Add the attribute to the operation state.
    // "value" is the name we'll use to refer to this in C++
    state.addAttribute("value", dataAttr);

    // 3. Set the result type of this operation
    // In MLIR, every SSA value must have a type.
    state.addTypes(dataType);
}

/*************************** BinOps *******************************************/
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
