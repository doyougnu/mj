# Tell TableGen where to find the source .td file
set(LLVM_TARGET_DEFINITIONS JOps.td)

find_program(MLIR_TABLEGEN_EXE NAMES mlir-tblgen REQUIRED)
message(STATUS "Tablegen: ${MLIR_TABLEGEN_EXE}")
# Define the generation rules
mlir_tablegen(JOps.h.inc -gen-op-decls
              -I ${MLIR_INCLUDE_DIRS}
              EXECUTABLE ${MLIR_TABLEGEN_EXE})

mlir_tablegen(JOps.cpp.inc -gen-op-defs
              -I ${MLIR_INCLUDE_DIRS}
              EXECUTABLE ${MLIR_TABLEGEN_EXE})

mlir_tablegen(JDialect.h.inc -gen-dialect-decls
              -I ${MLIR_INCLUDE_DIRS}
              EXECUTABLE ${MLIR_TABLEGEN_EXE})
# TODO
# mlir_tablegen(JTypes.h.inc -gen-typedef-decls -I ${MLIR_INCLUDE_DIRS})
# mlir_tablegen(JTypes.cpp.inc -gen-typedef-defs -I ${MLIR_INCLUDE_DIRS})

add_public_tablegen_target(MJTableGenTarget)
