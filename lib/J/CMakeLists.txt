add_mlir_library(MJLib
  JOps.cpp
  JDialect.cpp
  JParser.cpp
  JLexer.cpp
  JAstToMlir.cpp
  JSimplIdentities.cpp

  # Standard MLIR Dialects you are likely using
  LINK_LIBS PUBLIC
  MLIRIR
  MLIRSideEffectInterfaces
  MLIRArithDialect
  MLIRFuncDialect
)

# CRITICAL: Ensures the .inc files are generated before these .cpp files compile
add_dependencies(MJLib MJTableGenTarget)

# Explicitly link your generated include path
target_include_directories(MJLib PUBLIC
  ${PROJECT_BINARY_DIR}/include
)
