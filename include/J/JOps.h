#ifndef J_JOPS_H
#define J_JOPS_H

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"

namespace j {

/// The Plus operation (+) in J.
/// In J, '+' is a dyad that performs element-wise addition.
/// It supports "Rank Agreement," but for this C++ header, we define
/// the interface for MLIR to track the operation.
class PlusOp : public mlir::Op<PlusOp,
    mlir::OpTrait::AtLeastNOperands<2>::Impl,
    mlir::OpTrait::OneResult,
    mlir::OpTrait::AlwaysSpeculatableImplTrait,
    mlir::ConditionallySpeculatable::Trait,
    mlir::MemoryEffectOpInterface::Trait> {
public:
    // Inherit constructors from the base Op class
    using Op::Op;

    // The unique name for this operation in the J dialect
    static llvm::StringRef getOperationName() { return "j.plus"; }

    // This is the "Builder" used by JParser.cpp
    static void build(mlir::OpBuilder &builder, mlir::OperationState &state,
                      mlir::Value lhs, mlir::Value rhs);

    // Helpers to access operands
    mlir::Value getLHS() { return getOperation()->getOperand(0); }
    mlir::Value getRHS() { return getOperation()->getOperand(1); }

    // Mandatory: Logical validation of the operation
    mlir::LogicalResult verify();
};

// You can add more Ops here (e.g., NegateOp, ShapeOp) as your compiler grows.

} // namespace j

#endif // J_JOPS_H
