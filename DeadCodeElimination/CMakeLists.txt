add_llvm_library(LLVMDeadCodeElimination MODULE
    DeadCodeElimination.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)
