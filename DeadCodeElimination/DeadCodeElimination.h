#include "llvm/Pass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Locals.h"

namespace llvm {

    class DeadCodeElimination : public FunctionPass {
    public:
        static char ID;

        DeadCodeElimination() : FunctionPass(ID) {}

        void getAnalysisUsage(AnalysisUsage&) const override;
        bool runOnFunction(Function&) override;
    };

} /* anonymous namespace end */
