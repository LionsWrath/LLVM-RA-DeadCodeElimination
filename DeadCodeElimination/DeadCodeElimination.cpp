#define DEBUG_TYPE "ra-dce"

#include "DeadCodeElimination.h"

using namespace llvm;

STATISTIC(InstructionsEliminated, "Number of Instructions Eliminated");
STATISTIC(BasicBlocksEliminated, "Number of Basic Blocks Eliminated");

void DeadCodeElimination::solveICMPInstruction(BranchInst* BI, ICmpInst* I) {
    Range range1 = RA_->getRange(I->getOperand(0));
    Range range2 = RA_->getRange(I->getOperand(1));

    //errs() << "Operation: ";

    switch (I->getPredicate()) {
    case CmpInst::ICMP_EQ:  /* Equal */
        //errs() << "Equal\n";
        if (range1 == range2 && range1.getUpper().eq(range1.getLower())) {
            modifyBranchInst(BI, 0); 
        } else if (range1.getLower().ugt(range2.getUpper()) ||
                range1.getUpper().ult(range2.getLower())) {
            modifyBranchInst(BI, 1);
        }
        break;
    case CmpInst::ICMP_NE:  /* Not Equal */
        //errs() << "Not Equal\n";
        if (range1.getLower().ugt(range2.getUpper()) ||
                range1.getUpper().ult(range2.getLower())) {
            modifyBranchInst(BI, 0);
        } else if (range1 == range2 && 
                range1.getUpper().eq(range1.getLower())) {
            modifyBranchInst(BI, 1);
        }
        break;
    case CmpInst::ICMP_UGT: /* Unsigned Greater Than */
        //errs() << "Unsigned Greater Than\n";
        if (range1.getLower().ugt(range2.getUpper())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getUpper().ule(range2.getLower())) {
            modifyBranchInst(BI, 1); 
        }
        break;
    case CmpInst::ICMP_UGE: /* Unsigned Greater or Equal */
        //errs() << "Unsigned Greater or Equal\n";
        if (range1.getLower().uge(range2.getUpper())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getUpper().ult(range2.getLower())) {
            modifyBranchInst(BI, 1); 
        }
        break;
    case CmpInst::ICMP_ULT: /* Unsigned Less Than */
        //errs() << "Unsigned Less Than\n";
        if (range1.getUpper().ult(range2.getLower())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getLower().uge(range2.getUpper())) {
            modifyBranchInst(BI, 1); 
        }
        break;
    case CmpInst::ICMP_ULE: /* Unsigned Less or Equal */
        //errs() << "Unsigned Less of Equal\n";
        if (range1.getUpper().ule(range2.getLower())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getLower().ugt(range2.getUpper())) {
            modifyBranchInst(BI, 1); 
        }
        break;
    case CmpInst::ICMP_SGT: /* Signed Greater Than */
        //errs() << "Signed Greater Than\n";
        if (range1.getLower().sgt(range2.getUpper())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getUpper().sle(range2.getLower())) {
            modifyBranchInst(BI, 1);
        }
        break;
    case CmpInst::ICMP_SGE: /* Signed Greater or Equal */
        //errs() << "Signed Greater or Equal\n";
        if (range1.getLower().sge(range2.getUpper())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getUpper().slt(range2.getLower())) {
            modifyBranchInst(BI, 1);
        }
        break;
    case CmpInst::ICMP_SLT: /* Signed Less Than */
        //errs() << "Signed Less Than\n";
        if (range1.getUpper().slt(range2.getLower())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getLower().sge(range2.getUpper())) {
            modifyBranchInst(BI, 1);
        }
        break;
    case CmpInst::ICMP_SLE: /* Signed Less or Equal */
        //errs() << "Signed Less or Equal\n";
        if (range1.getUpper().sle(range2.getLower())) {
            modifyBranchInst(BI, 0);
        } else if (range1.getLower().sle(range2.getLower())) {
            modifyBranchInst(BI, 1); 
        }
        break;
    default:
        break;
    }

    //errs() << "    Range 1: [" << range1.getLower() << ", " << range1.getUpper() << "]\n";
    //errs() << "    Range 2: [" << range2.getLower() << ", " << range2.getUpper() << "]\n";
}

void DeadCodeElimination::solveFCMPInstruction(BranchInst*, FCmpInst* I) {
    // Does our range analysis apply to floats? Nop.
}

void DeadCodeElimination::modifyBranchInst(BranchInst* BI, int D) {
    IRBuilder<> Builder(BI);

    // Get successors of conditional branch
    BasicBlock* Dest = BI->getSuccessor(D);
    BasicBlock* Remo = BI->getSuccessor(!D);

    // Remove predecessor
    Remo->removePredecessor(BI->getParent());

    // Create new branch instruction
    Builder.CreateBr(Dest);
    
    // Erase instruction from parent BB
    BI->eraseFromParent();
}

void DeadCodeElimination::removeUnreachableBasicBlocks(Function& F) {

    auto *EB = &(F.getEntryBlock());
    std::vector<BasicBlock*> toRemove;

    // Don't remove the BB while iterating!
    // This invalidate the iterator! >:(
    for (BasicBlock& BB : F) {

        // Remove if there is no predecessor and is not entry
        if (0 == pred_size(&BB) && EB != &BB){
            toRemove.push_back(&BB);
        }
    }

    for (BasicBlock* BB : toRemove) {

        // Replace all uses of Instruction to be eliminated by UndefValues
        for (Instruction& I : *BB) {
            if (I.use_empty()) 
                continue;

            I.replaceAllUsesWith(UndefValue::get(I.getType()));
        }

        // Notify all successors of the block to be removed
        for (auto *succ : successors(BB)) {
            succ->removePredecessor(BB);
        }
        
        InstructionsEliminated += BB->size();
        BasicBlocksEliminated++;

        BB->eraseFromParent();       
    }
}

void DeadCodeElimination::removeTriviallyDeadInstructions(Function& F) {
    
    std::vector<Instruction*> toRemove;

    do {
        toRemove.clear();
        for (BasicBlock& BB : F) {
            for (Instruction& I : BB) {
                if (isInstructionTriviallyDead(&I, nullptr)) {
                    toRemove.push_back(&I);
                }
            }
        }

        // Remove Instructions
        for (auto *I : toRemove) {
            InstructionsEliminated++;
            I->eraseFromParent();
        } 
    } while (toRemove.size());
}

void DeadCodeElimination::applyConstantFold(Function& F) {
    
    for (BasicBlock& BB : F) {
        ConstantFoldTerminator(&BB, true, nullptr, nullptr);       
    }

}

void DeadCodeElimination::mergeBasicBlocks(Function& F) {
    
    // Find a BB where it has one predecessor and its predecessor has
    // only one successor.
    
    BasicBlock* DestBB;

    do {
        DestBB = nullptr;
        for (BasicBlock& BB : F) {
            BasicBlock* PBB = BB.getUniquePredecessor();

            if (PBB) {
                if (succ_size(PBB) == 1) {
                    DestBB = &BB; 
                    break; 
                    // We cant find more than one because the operation will
                    // change the BB's and this can change the BB pointers
                }
            }
        }

        if (DestBB) {
            MergeBasicBlockIntoOnlyPred(DestBB, nullptr);
        }
    } while (DestBB);
}

int DeadCodeElimination::instNumber(Function& F) {
    
    int counter = 0;
    for (BasicBlock& BB : F)
        counter += BB.size();

    return counter;
}

void DeadCodeElimination::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<InterProceduralRA<Cousot>>();
}

bool DeadCodeElimination::runOnFunction(Function &F) {

    BasicBlocksEliminated = F.size();
    InstructionsEliminated = instNumber(F); 

    RA_ = &getAnalysis<InterProceduralRA<Cousot>>();

    for (BasicBlock& BB : F) {
        
        auto T = BB.getTerminator();

        if (auto *BI = dyn_cast<BranchInst>(T)) {
            
            if (BI->isUnconditional()) 
                continue;

            if (auto *C = dyn_cast<ICmpInst>(BI->getCondition())) {
                solveICMPInstruction(BI, C);
            }
        }

        if (auto *SI = dyn_cast<SwitchInst>(T)) {
            // What to do if is a Switch?    
        }
    }

    // Remove trivially dead variables
    removeTriviallyDeadInstructions(F);

    // Apply Constant Fold
    applyConstantFold(F);

    // Remove unreachable Basic Blocks
    removeUnreachableBasicBlocks(F);

    // Merge useless Basic Block divisions
    mergeBasicBlocks(F);

    InstructionsEliminated -= instNumber(F);
    BasicBlocksEliminated -= F.size();

    errs() << "Function Name: " << F.getName() << "\n";
    errs() << "    Number of Eliminated Instructions: " << 
        InstructionsEliminated << "\n"; 
    errs() << "    Number of Basic Blocks completely removed (or moved): " << 
        BasicBlocksEliminated << "\n";

    return true;
}

char DeadCodeElimination::ID = 0;
static RegisterPass<DeadCodeElimination> X(
        "ra-dce",
        "Pass to Eliminate dead code using Range Analysis",
        true, /* TODO: Checar isso */ 
        true  /* TODO: Checar isso */
        );
