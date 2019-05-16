add_llvm_library(LLVMVSSA MODULE
    vSSA.cpp

    DEPENDS
    intrinsics_gen

    PLUGIN_TOOL
    opt
)
