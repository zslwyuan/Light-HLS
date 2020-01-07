This example of Light-HLS is **[Hi-ClockFlow](https://github.com/zslwyuan/Hi-ClockFlow/)**, to show how to implement a design space exploration with Light-HLS

Here, We mainly consider the parameters of clock frequencies of each module, array partitioning, loop pipelining and loop unrolling.


run with the command: 

       ./Hi_ClockFlow  <C/C++ FILE> <top_function_name>  <configuration_file> [DEBUG]

The main flow of Hi-ClockFlow is implememnted in [Hi_ClockFlow.cc](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc). After [pre-processing of source code](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L45) and [analysis of the IR code](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L76-#100), Hi-ClockFlow begins [its PUSH-RELAX flow](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L133-#404), where it tries to PUSH the HLS directives and when PUSH trial failed, it will update the clock combination by RELAX. Please note that these decision is made by these [lines](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L262-#402).

The **PUSH** operation is implemented with function [tryUpdateHLSDirectives()](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/ConfigGen.cc#L1025-L1292), in which Hi-ClockFlow find next combination of directives, including loop unrolling, loop pipelining and array partitioning, for the design to optimize the multi-clock dataflow performance. 

The **RELAX** operation is implemented with function [justTryUpdateSlowestFuncClock_withoutHLSDSE()](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/ConfigGen.cc#L1384-L1474), in which Hi-ClockFlow uses function [findNextPossibleClockCombination()](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/ConfigGen.cc#L1476-L1584) to get next combination of clocks for the dataflow design.

We keep updating the strategy of this heuristic algorithm for the setting of directives and clocks, ^_^/.