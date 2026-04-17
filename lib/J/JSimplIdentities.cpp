#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include <llvm/Support/LogicalResult.h>
#include <mlir/IR/PatternMatch.h>

#include "J/JOps.h"
#include "J/MJDialect.h"

#include "mlir/IR/PatternMatch.h"

/**** TODO: rewrite the match using this helper, like this:
// Inside matchAndRewrite:
using namespace mlir;
Attribute attr;

// This replaces the "getDefiningOp" + "dyn_cast" boilerplate
if (matchPattern(lhs, m_Constant(&attr))) {
    if (auto intAttr = llvm::dyn_cast<IntegerAttr>(attr)) {
        if (intAttr.getInt() == 0) {
            // Identity match!
            rewriter.replaceOp(op, rhs);
            return success();
        }
    }
    }
****/

namespace mj {

/*************************** Plus Identities **********************************/

struct LeftPlusIdentity : public mlir::OpRewritePattern<DyadAppOp> {

  LeftPlusIdentity(mlir::MLIRContext *context)
      : mlir::OpRewritePattern<DyadAppOp>(context) {};

  mlir::LogicalResult
  matchAndRewrite(DyadAppOp op,
                  mlir::PatternRewriter &rewriter) const override {

    mlir::Value verb_val = op.getVerb();
    auto verb = verb_val.getDefiningOp<VerbOp>();
    if (!verb) {
      return mlir::failure();
    }

    auto lhs = op.getLhs();
    auto rhs = op.getRhs();

    if (verb.getGlyph() != Prim::Plus) {
      return mlir::failure();
    }

    auto isConstZero = [](const mlir::Value v) -> bool {
      bool result = false;
      auto constOp = v.getDefiningOp<ConstantOp>();
      if (constOp) {
        auto attr = llvm::dyn_cast<mlir::IntegerAttr>(constOp.getValue());
        if (attr && attr.getInt() == 0) {
          result = true;
        }
      }
      return result;
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

}; // namespace mj
