class PlusOp : public mlir::Op<PlusOp,
    mlir::OpTrait::AtLeastNOperands<2>::Impl,
    mlir::OpTrait::OneResult> {
public:
  using Op::Op;
  static llvm::StringRef getOperationName() { return "j.plus"; }

  // This is where your Pratt Parser will "land"
  static void build(mlir::OpBuilder &builder, mlir::OperationState &state,
                   mlir::Value lhs, mlir::Value rhs);
};
