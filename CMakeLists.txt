cmake_minimum_required(VERSION 3.4.3)
project(RADeadCodeElimination)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

set(LLBM_LINK_COMPONENTS Core Support Transforms)

add_subdirectory(RangeAnalysis)
add_subdirectory(VSSA)
add_subdirectory(DeadCodeElimination)
