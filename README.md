# LLVM CFG PRINTER

## Information

* LLVM version: `9.0.0svn`;
* CMake version: `3.13.4`;

## About the Range Analysis

* The Range Analysis was modified for the LLVM 9.0.0;
* The Profile Class was reimplemented and it's working correctly, printing the running times in seconds;
* The Range Analysis and vSSA passes are now separated in their own passes.

## Compilation

### Compilation outside the LLVM

1. `export LLVM_DIR=<path to llvm build>`
1. `cd <printer base dir>`
1. `mkdir build`
1. `cd build`
1. `cmake ..`
1. `make`

### Compilation inside the LLVM

To add the Pass to the llvm compilation tree:

1. `cp RADeadCodeElimination <path to llvm source>/llvm/lib/Transforms/`
1. Add `add_subdirectory(RADeadCodeElimination)` to `<path to llvm source>/llvm/lib/Transforms/CMakeLists.txt`

Then, we can compile the llvm normally (now with our pass included):

1. `cd <path to llvm source>`
1. `mkdir build`
1. `cd build`
1. `cmake .. -G "Unix Makefiles"`
1. `make`

## Execution

First, we need to create the intermediate representation of the program:

`clang <input>.c -o <output_1>.bc -c -emit-llvm -O0`
`opt -instnamer -mem2reg -break-crit-edges <output_1>.bc -o <output_2>.rbc`

### Outside the LLVM compilation

`opt -load VSSA/LLVMVSSA.so -vssa <output_2>.rbc -o <output_3>.vssa.rbc`
`opt -load DeadCodeElimination/LLVMDeadCodeElimination.so -ra-dce <output_3>.vssa.rbc -o <output_4>.dce.rbc`

### Inside the LLVM compilation

`opt -vssa <output_2>.rbc -o <output_3>.vssa.rbc`
`opt -ra-dce <output_3>.vssa.rbc -o <output_4>.dce.rbc`
