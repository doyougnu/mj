#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include <llvm/Support/LogicalResult.h>
#include <mlir/IR/PatternMatch.h>

#include <variant>

#include "J/Ast.h"
#include "J/JOps.h"

namespace j {

/*************************** Plus Identities **********************************/

// START: implement over j::DyadOp for our own dialect
struct LeftPlusIdentity : public mlir::OpRewritePattern<j::DyadApp> {

  LeftPlusIdentity(mlir::MLIRContext *context)
      : mlir::OpRewritePattern<j::DyadApp>(context) {};

  mlir::LogicalResult matchAndRewrite(j::DyadApp op,
                                      mlir::PatternRewriter &rewriter) const {
    // auto verb ::std::move(op.verb);
    auto lhs = std::move(op.lhs);
    auto rhs = std::move(op.rhs);

    auto isConstZero = [](const j::ExprPtr &v) -> bool {
      return std::visit(
          Expr::cases{[](const j::IntLit &i) { return i.value == 0; },
                      [](const auto &) { return false; }},
          v->kind);
    };

    // TODO: change this to semigroup
    auto isPlusOp = [](const j::ExprPtr &operand) -> bool {
      if (!operand)
        return false;

      return std::visit(Expr::cases{[](const j::PrimVerb &p) {
                                      return p.glyph == j::Prim::Plus;
                                    },
                                    [](const auto &) { return false; }},
                        operand->kind);
    };
    // x + 0 --> x
    if (isConstZero(rhs)) {
      rewriter.replaceOp(op, lhs);
      return mlir::success();
    }

    // 0 + x --> x
    if (isConstZero(lhs)) {
      rewriter.replaceOp(op, rhs);
      return mlir::success();
    }

    return mlir::failure();
  }
};

}; // namespace j
