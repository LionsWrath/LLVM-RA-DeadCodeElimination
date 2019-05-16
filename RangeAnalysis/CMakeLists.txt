add_llvm_library(LLVMRangeAnalysis MODULE
    ClientRA.cpp
    RangeAnalysis.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)
