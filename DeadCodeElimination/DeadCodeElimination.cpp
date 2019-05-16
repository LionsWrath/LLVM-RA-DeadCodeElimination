#include "DeadCodeElimination.h"

using namespace llvm;

STATISTIC(InstructionsEliminated, "Number of Instructions Eliminated");
STATISTIC(BasicBlocksEliminated, "Number of Basic Blocks Eliminated");

void DeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const override {
    
}

bool DeadCodeElimination::runOnFunction(Function &F) override {

}

char DeadCodeElimination::ID = 0;
static RegisterPass<DeadCodeElimination> X(
        "dead-code-elimination",
        "Pass to Eliminate dead code using Range Analysis",
        true, /* TODO: Checar isso */ 
        true  /* TODO: Checar isso */
        );
