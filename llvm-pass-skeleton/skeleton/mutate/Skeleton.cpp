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
#include <vector>
#include <map>

using namespace llvm;
using namespace std;

namespace {

    struct SkeletonPass : public FunctionPass {
        static char ID;
        SkeletonPass() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) {
            errs() << F.getName() << "\n";
            for (auto &B : F) {

                errs() << "Basic block:\n";
                bool duplicate = false;
                int count  = 0;
                map<Instruction*, Instruction*> vars;
                vector<Instruction *> duplicate_block;
                bool skip = false;
                if(F.getName() == "AES_set_encrypt_key")
                {
                    errs() << F.getName() << "\n";
                    duplicate = true;
                }
                for (auto &I : B) {

                    if(!skip) {
                        srand(time(NULL));
                        int r_num = rand() % 2;


                        auto *ci = dyn_cast<CallInst>(&I);

                        /*
                        if (ci != NULL) {

                            // Insert at the point where the instruction `op` appears.

                            errs() << ci->getCalledFunction()->getName() << "\n";
                            if (ci->getCalledFunction()->getName() == "aes_dummy_start") {
                                count = 0;
                                duplicate = true;
                                continue;
                            }

                            if (ci->getCalledFunction()->getName() == "aes_dummy_stop") {
                                count = 0;
                                duplicate = false;
                                continue;
                            }
                        }
                        */

                        if (duplicate) {
                            auto new_inst = I.clone();
                            //If the cloned instruction writes to a global, modify it's value

                            if (r_num == 0) {
                                new_inst->insertBefore(&I);
                            } else {
                                new_inst->insertAfter(&I);
                                skip = true;
                            }

                            auto t_var = I.getName();
                            if(t_var!=""){
                                errs() << t_var << "\n";
                                auto dummy_name = t_var + "_dummy";
                                new_inst->setName(dummy_name);
                                errs() << "Name change " << t_var << " to " << new_inst->getName() << "\n";
                            }

                            vars[&I] = new_inst;
                            errs() << "Inserted " << I << "and" << *new_inst << "\n";

                            for (int i = 0; i < new_inst->getNumOperands(); i++) {
                                auto *typ = new_inst->getOperand(i);
                                Instruction *inst_op = dyn_cast<Instruction>(typ);
                                if (inst_op != NULL) {
                                    auto iter = vars.find(inst_op);
                                    if (iter != vars.end()) {
                                        errs() << "Found match " << *iter->second << "\n";
                                        new_inst->setOperand(i, iter->second);
                                    }
                                }



                                StringRef t_var = typ->getName();
                                if(t_var!="")
                                {
                                    errs() << t_var << "\n";
                                    auto replace_var = typ;
                                    //Replace var here, should be simple enough
                                }
                                errs() << "New_Inst op replaced" << *typ << " with " << *new_inst->getOperand(i)
                                       << "\n";
                            }

                            //new_inst->insertBefore(&I);

                        }
                    } else skip = false;


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
