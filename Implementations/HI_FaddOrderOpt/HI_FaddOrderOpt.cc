#include "HI_FaddOrderOpt.h"
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
#include "llvm/Transforms/Utils/Local.h"
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace llvm;

bool HI_FaddOrderOpt::runOnFunction(llvm::Function &F) // The runOnModule declaration will overide the virtual one in
                                                       // ModulePass, which will be executed for each Module.
{
    print_status("Running HI_FaddOrderOpt pass.");
    std::set<long long> power2;
    generatedI.clear();
    op2Cnt.clear();

    for (int i = 0, pow = 1; i < 64; i++, pow *= 2)
        power2.insert(pow);

    if (F.getName().find("llvm.") != std::string::npos)
        return false;

    // AAAAABB
    // AAAAA * BB
    // AA * AA * A   B *  B
    //  A * A
    bool changed = 0;
    for (BasicBlock &B : F)
    {
        bool action = 1;
        while (action)
        {
            action = 0;
            for (BasicBlock::reverse_iterator it = B.rbegin(), ie = B.rend(); it != ie; it++)
            {
                Instruction *I = &*it;

                if (I->getOpcode() != Instruction::FAdd)
                    continue;
                if (generatedI.find(I) != generatedI.end()) // this Instruction in generated by us, bypass it
                    continue;
                FPMathOperator *curFAddI = dyn_cast<FPMathOperator>(I);
                FPMathOperator *LFAdd = nullptr, *RFAdd = nullptr;
                Value *VarForFAdd;

                LFAdd = dyn_cast<FPMathOperator>(I->getOperand(0));
                RFAdd = dyn_cast<FPMathOperator>(I->getOperand(1));
                if (!LFAdd && !RFAdd)
                    continue;
                if (LFAdd->getOpcode() != Instruction::FAdd || RFAdd->getOpcode() != Instruction::FAdd)
                    continue;

                while (heap_opCnt.size() > 0)
                    heap_opCnt.pop();
                op2Cnt.clear();
                recursiveGetFAddOpAndCounter(I);

                int tot_cnt = 0;
                for (auto total_val_cnt_pair : op2Cnt[curFAddI])
                {
                    heap_opCnt.push(std::pair<int, Value *>(total_val_cnt_pair.second, total_val_cnt_pair.first));
                    tot_cnt += total_val_cnt_pair.second;
                }

                changed = 1;
                action = 1;

                *FAddOrderOptLog << "\n\nbefore replacement:\n\n" << B;
                IRBuilder<> Builder(I);

                int replcaceCnt = 0;

                Value *newFAdd = recursiveFAdd(heap_opCnt, tot_cnt, Builder);
                curFAddI->replaceAllUsesWith(newFAdd);
                RecursivelyDeleteTriviallyDeadInstructions(curFAddI);

                *FAddOrderOptLog << "\n\nafter replacement#" << replcaceCnt << ":\n\n" << B;
                // while (1)
                // {
                //     replcaceCnt++;
                //     int FAddcnt = (heap_opCnt.top()).first;
                //     if (FAddcnt>1)
                //     {
                //         *FAddOrderOptLog << "\n\n========================\n  handling " <<
                //         *heap_opCnt.top().second << "^(" << FAddcnt << ")\n";
                //         FAddOrderOptLog->flush();
                //         Value *newFAdd =
                //         heap_opCnt.pop();
                //         heap_opCnt.push(std::pair<int, Value*>(1, newFAdd));
                //         *FAddOrderOptLog << "\n\nafter replacement#" << replcaceCnt <<":\n\n" <<
                //         B;
                //     }
                //     else
                //     {
                //         if (heap_opCnt.size()==1)
                //         {
                //             *FAddOrderOptLog << "\n\n========================\n  do the
                //             re-FAddtiplication at " << *heap_opCnt.top().second << "\n";
                //             FAddOrderOptLog->flush();
                //             curFAddI->replaceAllUsesWith(heap_opCnt.top().second);
                //             RecursivelyDeleteTriviallyDeadInstructions(curFAddI);
                //             *FAddOrderOptLog << "\n\nafter replacement#" << replcaceCnt <<":\n\n"
                //             << B; break;
                //         }
                //         else
                //         {
                //             Value *val0 = heap_opCnt.top().second;
                //             heap_opCnt.pop();
                //             Value *val1 = heap_opCnt.top().second;
                //             heap_opCnt.pop();
                //             Value *newFAdd = Builder.CreateFAdd(val0, val1);
                //             generatedI.insert(newFAdd);
                //             *FAddOrderOptLog << "\n\n========================\n  do the
                //             re-FAddtiplication between " << *val0 << " and " << *val1 << " by " <<
                //             *newFAdd << "\n"; heap_opCnt.push(std::pair<int, Value*>(1, newFAdd));
                //             *FAddOrderOptLog << "\n\nafter replacement#" << replcaceCnt <<":\n\n"
                //             << B;
                //         }
                //     }
                // }

                break;

                if (action)
                    break;
            }
        }
    }
    FAddOrderOptLog->flush();
    return changed;
}

char HI_FaddOrderOpt::ID = 0; // the ID for pass should be initialized but the value does not matter, since LLVM uses
                              // the address of this variable as label instead of its value.

void HI_FaddOrderOpt::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.addRequired<DominatorTreeWrapperPass>();
    AU.addRequired<ScalarEvolutionWrapperPass>();
    AU.addRequired<LoopInfoWrapperPass>();
    AU.setPreservesCFG();
}

void HI_FaddOrderOpt::recursiveGetFAddOpAndCounter(Value *FAddI)
{
    if (auto real_FAddI = dyn_cast<FPMathOperator>(FAddI))
    {
        if (real_FAddI->getOpcode() != Instruction::FAdd)
            return;
        if (op2Cnt.find(real_FAddI) != op2Cnt.end())
            return;
        std::map<Value *, int> tmp_val_cnt_map;
        for (int i = 0; i < real_FAddI->getNumOperands(); i++)
        {
            FPMathOperator *opFAdd = dyn_cast<FPMathOperator>(real_FAddI->getOperand(i));
            if (opFAdd)
            {
                if (opFAdd->getOpcode() != Instruction::FAdd)
                    return;
                recursiveGetFAddOpAndCounter(opFAdd);
                for (auto val_cnt_pair : op2Cnt[opFAdd])
                {
                    if (tmp_val_cnt_map.find(val_cnt_pair.first) == tmp_val_cnt_map.end())
                    {
                        tmp_val_cnt_map[val_cnt_pair.first] = val_cnt_pair.second;
                    }
                    else
                    {
                        tmp_val_cnt_map[val_cnt_pair.first] += val_cnt_pair.second;
                    }
                }
            }
            else
            {
                if (tmp_val_cnt_map.find(real_FAddI->getOperand(i)) == tmp_val_cnt_map.end())
                {
                    tmp_val_cnt_map[real_FAddI->getOperand(i)] = 1;
                }
                else
                {
                    tmp_val_cnt_map[real_FAddI->getOperand(i)] += 1;
                }
            }
        }

        op2Cnt[real_FAddI] = tmp_val_cnt_map;
        *FAddOrderOptLog << "\n=========================\nop info for FAddI: " << *FAddI << "\n";
        for (auto val_cnt_pair : tmp_val_cnt_map)
        {
            *FAddOrderOptLog << "     -->" << *val_cnt_pair.first << "  --> " << val_cnt_pair.second << "\n";
        }
    }

    FAddOrderOptLog->flush();
}

Value *HI_FaddOrderOpt::recursiveFAdd(
    std::priority_queue<std::pair<int, Value *>, std::vector<std::pair<int, Value *>>, HI_FaddOrderOpt::cmp_faddorder>
        cur_heap,
    int tot_cnt, IRBuilder<> &Builder)
{
    std::priority_queue<std::pair<int, Value *>, std::vector<std::pair<int, Value *>>, cmp_faddorder> cur_heapL,
        cur_heapR;
    int cntL = (tot_cnt + 1) / 2;
    int cntR = tot_cnt - cntL;

    int const_cntL = cntL;
    int const_cntR = cntR;

    if (tot_cnt == 1)
        return cur_heap.top().second;

    while (cur_heap.size())
    {
        std::pair<int, Value *> curFAdds = cur_heap.top();

        cur_heap.pop();

        *FAddOrderOptLog << " spliting " << curFAdds.first << " x [" << *curFAdds.second << "]\n";

        if (curFAdds.first <= cntL)
        {
            cur_heapL.push(curFAdds);
            cntL -= curFAdds.first;
            *FAddOrderOptLog << "       assign " << curFAdds.first << " in left heap (newcap=" << cntL << ")\n";
        }
        else
        {
            if (cntL)
            {
                cur_heapL.push(std::pair<int, Value *>(cntL, curFAdds.second));
                *FAddOrderOptLog << "       assign " << cntL << " in left heap (newcap=" << 0 << ")\n";
                curFAdds.first -= cntL;
                cntL = 0;
            }

            cur_heapR.push(std::pair<int, Value *>(curFAdds.first, curFAdds.second));
            cntR -= (curFAdds.first);
            *FAddOrderOptLog << "       assign " << curFAdds.first << " in right heap (newcap=" << cntR << ")\n";
        }
        assert(cntL >= 0);
        assert(cntR >= 0);
    }

    *FAddOrderOptLog << "\n\n\n\n\n";
    Value *subFAdd0 = recursiveFAdd(cur_heapL, const_cntL, Builder);
    Value *subFAdd1 = recursiveFAdd(cur_heapR, const_cntR, Builder);
    Value *newFAdd = Builder.CreateFAdd(subFAdd0, subFAdd1);
    generatedI.insert(newFAdd);
    return newFAdd;
}