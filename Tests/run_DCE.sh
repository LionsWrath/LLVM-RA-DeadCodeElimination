#!/bin/bash

if [ $# -lt 1 ]
then
    echo Syntax: run_RA file.c
    exit 1
else
    CLANG="clang"
    OPT="opt"

    BUILD_DIR="../build"
    DCE_LIB="$BUILD_DIR/DeadCodeElimination/LLVMDeadCodeElimination.so"
    VSSA_LIB="$BUILD_DIR/VSSA/LLVMVSSA.so"

    file_name=$1
    base_name=$(basename $1 .c)

    orig_btcd_name="$base_name.orig.bc"
    new_btcd_name="$base_name.rbc"
    vssa_btcd_name="$base_name.vssa.rbc"
    dce_btcd_name="$base_name.dce.rbc"

    # Produce a bytecode file:
    $CLANG -Xclang -disable-O0-optnone $file_name -o $orig_btcd_name -c -emit-llvm

    # Convert the bytecode to SSA form, find nice names for variables and
    # break critical edges:
    $OPT -instnamer -mem2reg -break-crit-edges $orig_btcd_name -o $new_btcd_name

    # Break live ranges after conditionals to improve precision:
    $OPT -load $VSSA_LIB -vssa $new_btcd_name -o $vssa_btcd_name

    # Run the dce with range analysis
    $OPT -load $DCE_LIB -ra-dce $vssa_btcd_name -o $dce_btcd_name 

    # Producing a dot file for the vssa version of the bytecode file:
    $OPT -dot-cfg $vssa_btcd_name -disable-output

    rm -f $orig_btcd_name
    rm -f $new_btcd_name
    rm -f $vssa_btcd_name
    rm -f $dce_btcd_name
fi
