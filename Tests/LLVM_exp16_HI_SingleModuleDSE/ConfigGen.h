#ifndef _ConfigGen
#define _ConfigGen

#include "HI_StringProcess.h"
#include "HI_SysExec.h"
#include "HI_print.h"
#include "LLVM_exp16_HI_SingleModuleDSE.h"
#include <assert.h>
#include <iostream>
#include <map>
#include <string>

std::string findSlowestLastLevelLoop_InSlowestTopLoop(
    std::map<std::string, std::string> &IRLoop2LoopLabel,
    std::map<std::string, HI_WithDirectiveTimingResourceEvaluation::timingBase> &LoopLabel2Latency,
    std::map<std::string, std::vector<std::string>> &LoopIRName2NextLevelSubLoopIRNames,
    std::map<std::string, int> &LoopIRName2Depth);

std::string findTheSlowestLoop(
    std::string cur_LoopIRName, std::map<std::string, std::string> &IRLoop2LoopLabel,
    std::map<std::string, HI_WithDirectiveTimingResourceEvaluation::timingBase> &LoopLabel2Latency,
    std::map<std::string, std::vector<std::string>> &LoopIRName2NextLevelSubLoopIRNames,
    std::map<std::string, int> &LoopIRName2Depth);

std::map<std::string, std::vector<int>> getPotentialLoopUnrollFactor(
    std::map<std::string, std::string> &IRLoop2LoopLabel,
    std::map<std::string, std::vector<std::string>> &LoopIRName2NextLevelSubLoopIRNames,
    std::map<std::string, int> &LoopIRName2Depth,
    std::map<std::string, int> &IRLoop2OriginTripCount);

int findNextUnrollFactor(std::string IRLoopName, int cur_loopUnrollFactor,
                         std::map<std::string, std::vector<int>> &loop2PotentialUnrollFactors);

#endif