find_program(MLIR_TABLEGEN_EXE NAMES mlir-tblgen REQUIRED)

# always make the J directory
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/J)

# tblgen main
set(LLVM_TARGET_DEFINITIONS J/JOps.td)

# Define the generation rules, the order is important
mlir_tablegen(J/MJAttrs.h.inc -gen-enum-decls -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(J/MJAttrs.cpp.inc -gen-enum-defs -I ${MLIR_INCLUDE_DIRS})

mlir_tablegen(J/JOps.h.inc -gen-op-decls -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(J/JOps.cpp.inc -gen-op-defs -I ${MLIR_INCLUDE_DIRS})

mlir_tablegen(J/MJDialect.h.inc -gen-dialect-decls -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(J/MJDialect.cpp.inc -gen-dialect-defs -I ${MLIR_INCLUDE_DIRS})

# TODO
# mlir_tablegen(J/JTypes.h.inc -gen-typedef-decls -I ${MLIR_INCLUDE_DIRS})
# mlir_tablegen(J/JTypes.cpp.inc -gen-typedef-defs -I ${MLIR_INCLUDE_DIRS})

add_public_tablegen_target(MJTableGenTarget)
