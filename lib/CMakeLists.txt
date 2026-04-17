add_mlir_library(MJLib
  J/JOps.cpp
  J/MJDialect.cpp
  J/JParser.cpp
  J/JLexer.cpp
  J/JAstToMlir.cpp
  J/JSimplIdentities.cpp

  # Standard MLIR Dialects you are likely using
  LINK_LIBS PUBLIC
  MLIRIR
  MLIRSideEffectInterfaces
  MLIRArithDialect
  MLIRFuncDialect

  # Ensures the .inc files are generated before these .cpp files compile
  DEPENDS
  MJTableGenTarget
)

# Explicitly link your generated include path
target_include_directories(MJLib PUBLIC
  ${PROJECT_BINARY_DIR}/include
)
