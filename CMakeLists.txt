cmake_minimum_required(VERSION 3.15)
project(j-mlir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

if(NOT CMAKE_BUILD_TYPE) # if the build type is not passed on the command line
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()



# 1. Locate Packages
find_package(MLIR REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)
include_directories("${MLIR_INCLUDE_DIRS}")
include_directories("${LLVM_INCLUDE_DIRS}")

# 2. Define the Dialect Library
# We group the source files here
add_library(JDialect
    lib/J/JDialect.cpp
    lib/J/JOps.cpp
    lib/J/JParser.cpp
    lib/J/JLexer.cpp
    lib/J/JAstToMlir.cpp
)

add_executable(mjc ${CMAKE_CURRENT_SOURCE_DIR}/tools/mjc/mjc.cpp)

# Use the variables defined by find_package OR passed via -D
target_include_directories(JDialect PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${MLIR_INCLUDE_DIRS}"
    "${LLVM_INCLUDE_DIRS}"
)

target_include_directories(mjc SYSTEM PRIVATE
    "${MLIR_INCLUDE_DIRS}"
    "${LLVM_INCLUDE_DIRS}"
    "${PROJECT_SOURCE_DIR}/include"
)

# 4. Link Libraries
# MLIR targets often carry their own include requirements
target_link_libraries(JDialect
    PUBLIC
    MLIRIR
    MLIRArithDialect
    MLIRFuncDialect
    MLIRParser
    MLIRSupport
    MLIRLinalgDialect
)

target_link_libraries(mjc PRIVATE
    JDialect
    MLIROptLib
    MLIRSupport
)

# Force -O0 for Debug builds
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0" CACHE STRING "Debug flags" FORCE)
