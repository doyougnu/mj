#ifndef J_JSIMPLIDENTITIES_H
#define J_JSIMPLIDENTITIES_H

#include "J/Ast.h"
#include "J/JOps.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"
#include "mlir/Interfaces/InferTypeOpInterface.h"
#include "mlir/Interfaces/SideEffectInterfaces.h"
#include <llvm/Support/LogicalResult.h>
#include <mlir/IR/PatternMatch.h>

namespace j {

/*************************** Plus Identities **********************************/

struct LeftPlusIdentity : public mlir::OpRewritePattern<mlir::arith::AddIOp> {

  LeftPlusIdentity(mlir::MLIRContext *context);

  mlir::LogicalResult
  matchAndRewrite(mlir::arith::AddIOp op,
                  mlir::PatternRewriter &rewriter) const override;
};

} // namespace j
#endif // J_JSIMPLIDENTITES_H
