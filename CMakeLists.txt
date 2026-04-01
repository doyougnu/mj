cmake_minimum_required(VERSION 3.15)
project(j-mlir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Locate Packages
# find_package(MLIR REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)
set(MLIR_INCLUDE_DIRS "$ENV{MLIR_INC_DIR}")
message(STATUS "MLIR Include Dirs: ${MLIR_INCLUDE_DIRS}")
message(STATUS "LLVM Include Dirs: ${LLVM_INCLUDE_DIRS}")
include_directories("${MLIR_INCLUDE_DIRS}")
include_directories("${LLVM_INCLUDE_DIRS}")

# 2. Define the Dialect Library
# We group the source files here
add_library(JDialect
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/J/JDialect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/J/JOps.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/J/JParser.cpp
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
