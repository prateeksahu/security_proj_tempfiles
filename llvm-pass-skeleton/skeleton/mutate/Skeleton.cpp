#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "iterator"
#include "llvm/ADT/ilist.h"
using namespace llvm;

namespace {
  struct SkeletonPass : public FunctionPass {
    static char ID;
    SkeletonPass() : FunctionPass(ID) {}

    virtual bool runOnFunction(Function &F) {
      for (auto &B : F) {
        errs() << "Basic block:\n";
        bool duplicate = false; 
        for (auto &I : B) {
          auto *ci = dyn_cast<CallInst>(&I);
          if (ci != NULL) {
            // Insert at the point where the instruction `op` appears.
            
            errs() << ci->getCalledFunction()->getName() << "\n";
            if (ci->getCalledFunction()->getName() == "aes_dummy"){
              duplicate = true;
            }
            // Make a multiply with the same operands as `op`.
            // Value *lhs = op->getOperand(0);
            // Value *rhs = op->getOperand(1);
            // Value *mul = builder.CreateMul(lhs, rhs);

            // Everywhere the old instruction was used as an operand, use our
            // new multiply instruction instead.
            // for (auto &U : op->uses()) {
            //   User *user = U.getUser();  // A User is anything with operands.
            //   user->setOperand(U.getOperandNo(), mul);
            // }

            // We modified the code.
            // return true;
          }
          if (auto *br = dyn_cast<TerminatorInst>(&I)){
            duplicate = false;
          }
          if (duplicate){
            if(auto *op = dyn_cast<CallInst>(&I)){
              if(op->getCalledFunction()->getName() == "printf"){
                errs() << "should duplicate here\n";
//                IRBuilder<> builder(ci);
                // builder.SetInsertPoint(&B, ++builder.GetInsertPoint());
                auto new_inst = op->clone();
//                op->getParent()->getInstList().push_back(new_inst);
                  new_inst->insertBefore(&I);
//                  op->getParent()->getInstList().insert(I, new_inst);
                // new_inst->setName(ci->getName());
                errs() << *(op->getArgOperand(0)) << "\n";
                  if( ConstantExpr * pCE = dyn_cast<ConstantExpr>( op->getArgOperand(0))){
                      errs() << *pCE << "\n";
                    if( GlobalVariable * pGV = dyn_cast<GlobalVariable>( pCE->getOperand(0))){
                        errs() << *pGV << "\n";
                        if( ConstantArray * pCA = dyn_cast<ConstantArray>(pGV->getInitializer())){
                            errs() << *pCA << "\n";
                        }
                    }
                  }
                // builder.Insert();
                errs() << *op << "\n";
                errs() << *new_inst << "\n";
              }
            }
          }
        }

          B.dump();
      }

      // return false;
    }
  };
}

char SkeletonPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerSkeletonPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new SkeletonPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerSkeletonPass);
