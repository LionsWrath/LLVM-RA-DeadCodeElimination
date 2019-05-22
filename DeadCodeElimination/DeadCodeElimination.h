#include "llvm/Pass.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include "../RangeAnalysis/RangeAnalysis.h"

namespace llvm {

    class DeadCodeElimination : public FunctionPass {
    private:
        InterProceduralRA<Cousot>* RA_;

        void solveICMPInstruction(BranchInst*, ICmpInst*);
        void solveFCMPInstruction(BranchInst*, FCmpInst*);
        void modifyBranchInst(BranchInst*, int);
        void removeUnreachableBasicBlocks(Function&);
        void removeTriviallyDeadInstructions(Function&);
        void mergeBasicBlocks(Function&);

    public:
        static char ID;

        DeadCodeElimination() : FunctionPass(ID) {}

        void getAnalysisUsage(AnalysisUsage&) const override;
        bool runOnFunction(Function&) override;
    };

}
