# DEAD CODE ELIMINATION

## Information

* LLVM version: `9.0.0svn`;
* CMake version: `3.13.4`;

## About the Range Analysis

* The `Range Analysis` was modified for the LLVM `9.0.0`;
* The `Profile Class` was reimplemented and it's working correctly, printing the running times in seconds;
* The `Range Analysis` and `vSSA` passes are now separated.

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

1. `cp -r VSSA <path to llvm source>/llvm/lib/Transforms/`
1. `cp -r RangeAnalysis <path to llvm source>/llvm/lib/Transforms/`
1. `cp -r DeadCodeElimination <path to llvm source>/llvm/lib/Transforms/`
1. Add `add_subdirectory(VSSA)` to `<path to llvm source>/llvm/lib/Transforms/CMakeLists.txt`
1. Add `add_subdirectory(RangeAnalysis)` to `<path to llvm source>/llvm/lib/Transforms/CMakeLists.txt`
1. Add `add_subdirectory(DeadCodeElimination)` to `<path to llvm source>/llvm/lib/Transforms/CMakeLists.txt`

Remove `MODULE`, `PLUGIN_TOOL` and the `opt` line from the CMakeList.txt in each of the copied dirs. Then, we can compile the llvm normally (now with our pass included):

1. `cd <path to llvm source>`
1. `mkdir build`
1. `cd build`
1. `cmake .. -G "Unix Makefiles"`
1. `make`

## Execution

First, we need to create the intermediate representation of the program:

1. `clang <input>.c -o <output_1>.bc -c -emit-llvm -O0`
1. `opt -instnamer -mem2reg -break-crit-edges <output_1>.bc -o <output_2>.rbc`

### Outside the LLVM compilation

1. `opt -load <build>/VSSA/LLVMVSSA.so -vssa <output_2>.rbc -o <output_3>.vssa.rbc`
1. `opt -load <build>/DeadCodeElimination/LLVMDeadCodeElimination.so -ra-dce <output_3>.vssa.rbc -o <output_4>.dce.rbc`

### Inside the LLVM compilation

1. `opt -vssa <output_2>.rbc -o <output_3>.vssa.rbc`
1. `opt -ra-dce <output_3>.vssa.rbc -o <output_4>.dce.rbc`

## Scripts

I have some scripts to help execution in the `Tests` folder:

* `run_DCE.sh`: Run the DCE for an .c file as argument;
* `test.sh`: Run the DCE for all tests in `Tests` folder;
* `show.sh`: Use `dot` to show a .dot file to pdf (may not be compatible);

Just rewrite the variables `CLANG`, `OPT` and `BUILD_DIR` to your configuration.
