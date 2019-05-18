add_llvm_library(LLVMDeadCodeElimination MODULE
    DeadCodeElimination.cpp
    ../RangeAnalysis/RangeAnalysis.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)
