#include "HI_TopLoop2Func.h"
#include "HI_StringProcess.h"
#include "HI_print.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

bool HI_TopLoop2Func::runOnModule(
    Module &M) // The runOnModule declaration will overide the virtual one in ModulePass, which will
               // be executed for each Module.
{
    print_status("Running HI_TopLoop2Func pass.");
    FuncNames.clear();
    for (auto &F : M)
    {
        if (F.getName().find("llvm.") != std::string::npos)
            continue;

        if (demangleFunctionName(F.getName().str()) != top_str ||
            F.getName().find(".") != std::string::npos)
            FuncNames.push_back(F.getName().str());

        auto &LI = getAnalysis<LoopInfoWrapperPass>(F).getLoopInfo();
        auto &SE = getAnalysis<ScalarEvolutionWrapperPass>(F).getSE();
        for (auto Loop : LI.getLoopsInPreorder())
        {
            if (Loop->getLoopDepth() == 1)
            {
                std::string tmp_loop_name = Loop->getHeader()->getParent()->getName().str();
                tmp_loop_name += "-";
                tmp_loop_name += Loop->getHeader()->getName().str();
                TopLoopIR2FuncName[tmp_loop_name] = Loop->getHeader()->getParent()->getName().str();
            }
        }
    }

    return false;
}

char HI_TopLoop2Func::ID =
    0; // the ID for pass should be initialized but the value does not matter, since LLVM uses the
       // address of this variable as label instead of its value.

void HI_TopLoop2Func::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequired<ScalarEvolutionWrapperPass>();
    AU.setPreservesCFG();
}