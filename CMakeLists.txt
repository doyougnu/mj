cmake_minimum_required(VERSION 3.15)
project(j-mlir LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(MLIR REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${LLVM_INCLUDE_DIRS})

# 2. Add the library
include_directories(include)
add_definitions(${LLVM_DEFINITIONS})
add_library(JDialect lib/J/JDialect.cpp lib/J/JOps.cpp lib/J/JParser.cpp)

# 3. Link the actual MLIR libraries
# This also pulls in the transitive header requirements for these libs
target_link_libraries(JDialect
    PUBLIC
    MLIRIR
    MLIRArith
    MLIRFunc
)

# 4. The executable
add_executable(mjc tools/mjc/mjc.cpp)
target_link_libraries(mjc PRIVATE
    JDialect
    MLIRIR
    MLIROptLib
    MLIRSupport
)
