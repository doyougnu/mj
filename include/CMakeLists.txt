# Tell TableGen where to find the source .td file
set(LLVM_TARGET_DEFINITIONS J/JOps.td)

find_program(MLIR_TABLEGEN_EXE NAMES mlir-tblgen REQUIRED)

# Define the generation rules
mlir_tablegen(JOps.h.inc -gen-op-decls -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(JOps.cpp.inc -gen-op-defs -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(MJDialect.h.inc -gen-dialect-decls -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(MJDialect.cpp.inc -gen-dialect-defs -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(MJAttrs.h.inc -gen-attrdef-decls -I ${MLIR_INCLUDE_DIRS})
mlir_tablegen(MJAttrs.cpp.inc -gen-attrdef-defs -I ${MLIR_INCLUDE_DIRS})
# mlir_tablegen(MJAttrs.h.inc -gen-enum-decls -I ${MLIR_INCLUDE_DIRS})
# mlir_tablegen(MJAttrs.cpp.inc -gen-enum-defs -I ${MLIR_INCLUDE_DIRS})

# TODO
# mlir_tablegen(JTypes.h.inc -gen-typedef-decls -I ${MLIR_INCLUDE_DIRS})
# mlir_tablegen(JTypes.cpp.inc -gen-typedef-defs -I ${MLIR_INCLUDE_DIRS})

add_public_tablegen_target(MJTableGenTarget)
