#include "mlir/IR/AsmState.h"
// Dialect Headers
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/Linalg/IR/Linalg.h"

// Standard MLIR Headers
#include "mlir/IR/DialectRegistry.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include <J/JLexer.h>

// J Dialect and friends
#include "J/JDialect.h"
#include "J/JParser.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        llvm::errs() << "Usage: mjc <filename>\n";
        return 1;
    }

    // 1. Setup Context and Registry
    mlir::DialectRegistry registry;
    registry.insert<mlir::arith::ArithDialect, mlir::func::FuncDialect,
                    mlir::memref::MemRefDialect, mlir::linalg::LinalgDialect,
                    j::JDialect>();

    mlir::MLIRContext context(registry);
    context.loadAllAvailableDialects();

    // 2. Load the input file
    llvm::SourceMgr sourceMgr;
    std::string filename = argv[1];
    auto fileOrErr = llvm::MemoryBuffer::getFile(filename);
    if (std::error_code ec = fileOrErr.getError()) {
        llvm::errs() << "Could not open input file: " << ec.message() << "\n";
        return 1;
    }
    sourceMgr.AddNewSourceBuffer(std::move(*fileOrErr), llvm::SMLoc());

    mlir::OpBuilder tempBuilder(&context);
    mlir::ModuleOp module = mlir::ModuleOp::create(tempBuilder.getUnknownLoc());
    mlir::OpBuilder builder(module.getBodyRegion());

    // initialize the lexer and parser
    j::JLexer lexer(sourceMgr);
    j::JParser parser(builder, lexer);

    // kickoff
    if (mlir::failed(parser.parse())) {
        llvm::errs() << "Error: Failed to parse J source.\n";
        return 1;
    }

    // 4. Print the generated MLIR to stdout
    module.dump(); // ->print(llvm::outs());

    return 0;
}
