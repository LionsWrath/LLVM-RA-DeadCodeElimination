#define DEBUG_TYPE "ra-dce"

#include "DeadCodeElimination.h"

using namespace llvm;

STATISTIC(InstructionsEliminated, "Number of Instructions Eliminated");
STATISTIC(BasicBlocksEliminated, "Number of Basic Blocks Eliminated");

void DeadCodeElimination::solveICMPInstruction(ICmpInst* I) {
    errs() << "Entrou Solver\n";
    
    Range range1 = RA_->getRange(I->getOperand(0));
    Range range2 = RA_->getRange(I->getOperand(1));

    errs() << "Range 1: [" << range1.getLower() << ", " << range1.getUpper() << "]\n";
    errs() << "Range 2: [" << range2.getLower() << ", " << range2.getUpper() << "]\n";

    switch (I->getPredicate()) {
    case CmpInst::ICMP_EQ:  /* Equal */
        errs() << "Equal\n";
        if (range1 == range2 && range1.getUpper().eq(range1.getLower())) {
            modifySolvedUses(I); 
        }
        break;
    case CmpInst::ICMP_NE:  /* Not Equal */
        errs() << "Not Equal\n";
        if (range1.getLower().ugt(range2.getUpper()) ||
                range1.getUpper().ult(range2.getLower())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_UGT: /* Unsigned Greater Than */
        errs() << "Unsigned Greater Than\n";
        if (range1.getLower().ugt(range2.getUpper())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_UGE: /* Unsigned Greater or Equal */
        errs() << "Unsigned Greater or Equal\n";
        if (range1.getLower().uge(range2.getUpper())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_ULT: /* Unsigned Less Than */
        errs() << "Unsigned Less Than\n";
        if (range1.getUpper().ult(range2.getLower())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_ULE: /* Unsigned Less or Equal */
        errs() << "Unsigned Less of Equal\n";
        if (range1.getUpper().ule(range2.getLower())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_SGT: /* Signed Greater Than */
        errs() << "Signed Greater Than\n";
        if (range1.getLower().sgt(range2.getUpper())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_SGE: /* Signed Greater or Equal */
        errs() << "Signed Greater or Equal\n";
        if (range1.getLower().sge(range2.getUpper())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_SLT: /* Signed Less Than */
        errs() << "Signed Less Than\n";
        if (range1.getUpper().slt(range2.getLower())) {
            modifySolvedUses(I);
        }
        break;
    case CmpInst::ICMP_SLE: /* Signed Less or Equal */
        errs() << "Signed Less or Equal\n";
        if (range1.getUpper().sle(range2.getLower())) {
            modifySolvedUses(I);
        }
        break;
    default:
        break;
    }
}

void DeadCodeElimination::solveFCMPInstruction(FCmpInst* I) {
    // Does our range analysis apply to floats?
}

void DeadCodeElimination::modifySolvedUses(Instruction* I) {
    errs() << "Entrou Modify\n";

    for (auto it = I->use_begin(); it != I->use_end(); ++it){

        auto* I = dyn_cast<Instruction>(it->get());
        IRBuilder<> Builder(I);
        
        if (isa<BranchInst>(I)) {
            errs() << "REMOVAL 1\n";
            auto *BI = dyn_cast<BranchInst>(I);

            // If this branch is uncoditional this really doesnt make sense
            if (BI->isUnconditional())
                return; /* SUPER ERROR */

            // Sanity check to see if the condition is really a CMP
            if (isa<CmpInst>(BI->getCondition())) {
                errs() << "REMOVAL 2\n";

                // Get successors of conditional branch
                BasicBlock* Dest1 = BI->getSuccessor(0);
                BasicBlock* Dest2 = BI->getSuccessor(1);

                // Remove predecessor
                Dest2->removePredecessor(BI->getParent());

                Builder.CreateBr(Dest1);
                BI->eraseFromParent();
            }
        }
    }
}

void DeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<InterProceduralRA<Cousot>>();
}

bool DeadCodeElimination::runOnFunction(Function &F) {

    RA_ = &getAnalysis<InterProceduralRA<Cousot>>();

    for (BasicBlock& BB : F) {
        for (Instruction& I : BB) {
            // Is an compare function?
            if (isa<CmpInst>(&I)) {
                auto *I_ = dyn_cast<CmpInst>(&I);

                if (I_->isIntPredicate())
                    solveICMPInstruction(dyn_cast<ICmpInst>(I_));
                else
                    solveFCMPInstruction(dyn_cast<FCmpInst>(I_));
            }
        }
    }

    return true;
}

char DeadCodeElimination::ID = 0;
static RegisterPass<DeadCodeElimination> X(
        "ra-dce",
        "Pass to Eliminate dead code using Range Analysis",
        true, /* TODO: Checar isso */ 
        true  /* TODO: Checar isso */
        );
