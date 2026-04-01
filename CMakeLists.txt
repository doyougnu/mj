cmake_minimum_required(VERSION 3.15)
project(j-mlir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1. Locate Packages
find_package(MLIR REQUIRED CONFIG)
include_directories(${MLIR_INCLUDE_DIRS})
find_package(LLVM REQUIRED CONFIG)

# 2. Define the Dialect Library
# We group the source files here
add_library(JDialect
    lib/J/JDialect.cpp
    lib/J/JOps.cpp
    lib/J/JParser.cpp
)

add_executable(mjc tools/mjc/mjc.cpp)

# Use the variables defined by find_package OR passed via -D
target_include_directories(JDialect PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MLIR_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
)

target_include_directories(mjc PRIVATE
    ${MLIR_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
)

# 4. Link Libraries
# MLIR targets often carry their own include requirements
target_link_libraries(JDialect
    PUBLIC
    MLIRIR
    MLIRArith
    MLIRFunc
)

target_link_libraries(mjc PRIVATE
    JDialect
    MLIROptLib
    MLIRSupport
)
