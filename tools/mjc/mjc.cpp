#include "mlir/IR/MLIRContext.h"
#include "mlir/InitAllDialects.h"
#include "J/JDialect.h"

int main(int argc, char **argv) {
    mlir::DialectRegistry registry;

    // Register standard MLIR dialects (func, arith, etc.)
    mlir::registerAllDialects(registry);

    // Register YOUR J dialect
    registry.insert<j::JDialect>();

    mlir::MLIRContext context(registry);
    context.loadAllAvailableDialects();

    // Now you can parse J and emit 'j.plus' ops!
    return 0;
}
