# Light-HLS

Light-HLS is a light weight HLS framework for academic exploration and evaluation, which can be called to perform various design space exploration (DSE) for FPGA-based HLS design. It covers the abilities of previous works, overcomes the existing limitations and brings more practical features. Light-HLS is modularized and portable so designers can use the components of Light-HLS to conduct various DSE procedures.  Light-HLS gets rid of RTL code generation so it will not suffer from the time-consuming synthesis of commercial HLS tools like VivadoHLS, which involves many detailed operations in both its frond-end and back-end.


<img src="https://github.com/zslwyuan/Light-HLS/blob/master/Images/Light-HLS-Overview.png" width="800"> 

## Light-HLS Frond-End

The goal of Light-HLS frond-end is to generate IR code close enough to those generated via commercial tools, like VivadoHLS, for DSE purpose. In the front-end of Light-HLS, initial IR codes generated via Clang will be processed by HLS optimization passes consisted of three different levels: (a) At instruction level, Light-HLS modifies, removes or reorders the instructions, e.g. reducing bitwidth,  removing redundant instruction  and reordering computation. (b) At loop/function level, functions will be instantiated and loops may be extracted into sub-functions. (c) As for memory access level, redundant load/store instructions will be removed based on dependency analysis.

## Light-HLS Back-End

The back-end of Light-HLS is developed to schedule and bind for the optimized IR codes, so it can predict the resultant performance and resource cost accurately based on the given settings. The IR instructions can be automatically characterized by Light-HLS and a corresponding library, which records the timing and resource of different types of operations, will be generated. For scheduling, based on the generated library, Light-HLS maps most operations to corresponding cycles based on as-soon-as-possible (ASAP) strategy. For some pipelined loops, the constraints of the port number of the BRAMs and loop-carried dependencies are considered. Moreover, some operations might be scheduled as late as possible (ALAP) to lower the II. 	As for resource binding, Light-HLS accumulates the resource cost by each operation and the chaining of operations is considered. The reusing of hardware resource is an important feature in HLS and Light-HLS reuses resources based on more detailed rules, e.g. the source and type of input.

## Category:

**[Usage of Light-HLS](https://github.com/zslwyuan/Light-HLS#usage-of-light-hls)**

**[List of Experiments I conducted](https://github.com/zslwyuan/Light-HLS#list-of-experiments-i-conducted)**

**[Further development](https://github.com/zslwyuan/Light-HLS#further-development)**


***


 
## [Usage of Light-HLS](https://github.com/zslwyuan/Light-HLS#usage-of-light-hls)

1. download the blog (entire project)
2. basic functiones and passes are implemented in the directory **["Implementations"](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations)**. Nearly all the directories have their own README file to explain the directory.
3. experiments are tested in the directory **["Test"](https://github.com/zslwyuan/Light-HLS/tree/master/Tests)**.
4. by making a "build" directory and using CMake in each experiment directory (e.g. **[this one](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/LLVM_exp5_SimpleTimingAnalysis/)**), executable can be generated and tried. (hint: cmake .. & make) 
5. for user's convenience, I prepare some scripts for example, **BuildAllFiles.sh**, which will build all the projects, **CleanBuiltFiles.sh**, which will clean all the built files to shrink the size of the directories, and **Build.sh** in test directory, which will just build one test project. All these scripts can be run directly.
6. looking into the source code with detailed comments, reader can trace the headers and functions to understand how the experiment work.




***



**usage example**

      When built, most test executables can be used like below but please check the source code for confirmation.
      (1) ./LLVM\_expXXXXX  <C/C++ FILE> <top\_function\_name>   
      or
      (2) ./LLVM\_expXXXXX  <IR FILE>


***

## [List of Experiments I conducted](https://github.com/zslwyuan/Light-HLS#list-of-experiments-i-conducted):

**Experiment 0**: Find all the functions' names in the program. (Analysis Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_FindFunctions)**

      -- keywords: installation of LLVM, basic pass template

**Experiment 1**: Get the dependence graph in a function and plot it. (Analysis Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_DependenceList)**

      -- keywords: dependence types, graph, iterator, passmanager, iterator, types in LLVM 

**Experiment 2**: Utilize existing LLVM passes in your project  (Analysis Pass)  **[TEST CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/LLVM_exp2_loop_processing)**

      -- keywords: invoke different passes in your application, createXXXXPass (or new xxxxPass), 

**Experiment 3**: Basic Loop information analysis  (Analysis Pass)  **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_LoopInFormationCollect)**

      -- keywords: co-operation/dependence between Pass, Basic function for loop analysis

**Experiment 4**: Invoke polyhedral Information Pass  (Analysis Pass)  **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_Polly_Info)**

      -- keywords: compilation with polly libraries, pass dependence

**Experiment 5**: Build a ASAP scheduler to schedule the instructions for parallel machine (e.g. FPGA) and evaluate latency (Analysis Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_SimpleTimingEvaluation)**

      -- keywords: information processing, DFS traverse, comprehensively used the Function/Loop/Block information, basic scheduler implementation
      -- PS: Experiment 5a is just used to collect involved instructions in a IR file.


**Experiment 6**: Analyze the GEP lowering transformation pass (Passed implemented in previous experimented are analysis passes.)  **[TEST CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/LLVM_exp6_GEP_Transformation)**

      -- SeparateConstOffsetFromGEP, GEP Lowering, Transformation Pass, DataLayout


**Experiment 7**: Implementation a transformation pass to safely remove duplicated instrcutions (Transformation Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_DuplicateInstRm)**

      -- keywords: erase an instruction, remove duplicatied instruction, eraseFromParent, replaceAllUsesWith

**Experiment 8**: By using SCEV, Successfully obtain the range of the targe value and then implement bitwidth optimization (IR insertion/operand replacement/IR removal/reduncdant instruction check T\_T....) (Transformation Pass)    **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_VarWidthReduce)** 

      -- keywords: SCEV, value range, bitwidth optimization, Usage of IR Builder, change type/width of value


**Experiment 9**: Build a GEP operation transformation pass for HLS  (Transformation Pass)  **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_SeparateConstOffsetFromGEP)**

      --- HLS, ByteAlignment->ElementAlignment, SeparateConstOffsetFromGEP, GEP Lowering, Transformation Pass, DataLayout
      --- WARNING: Currently, array of struct is not supported. In the future, we need to transform array of struct to seperate array.


**Experiment 10**: Hack on Clang to detect arbitary precision integer ap\_int<X> in source code for VivadoHLS and mark them in the source code (Front-end Operation: AST Consumer) **[ACTION CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_APIntSrcAnalysis)**

      --- Front-End Operation, AST Consumer, Visit Nodes in AST, Clang, Template Detection


**Experiment 11**: (Patch-Applied) Hack on Clang to properly map arbitary precision integer ap\_int<X> in source code for VivadoHLS into iX in IR generation  (Front-end Operation: CodeGen) **[TEST CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/LLVM_expAPINT_test)**. Since a patch for LLVM-9 necessary for this tutorial is still under the review of LLVM, **[complete source code of LLVM for this tutorial](https://github.com/zslwyuan/LLVM-9-for-Light-HLS)** can be downloaded via github in case that you cannot make the patch work properly. 

      --- CodeGen Operation, AST Consumer, Visit Nodes in AST, Clang
      --- The original way I implement is too ugly and I applied the patch which Erich Keane shared via maillist clang-cfe.
      --- The patch, under review: https://reviews.llvm.org/D59105 , is developed by Erich Keane, Compiler Engineer, Intel Corporation. (Thanks Erich!^_^)


**Experiment 12**: Based on the library collected from VivadoHLS synthesis result, a Pass for more accurate evaluation of timing and resource is implemented. (Analysis Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_NoDirectiveTimingResourceEvaluation)** 

      --- HLS Timing/Resource Library for most instructions, Timing/Resource Evaluation, Operation Chaining for DSP Utilization
	
**Experiment 12(a)**: Based on DominatorTree push instructions back to dominant ancestor to improve parallelism and reduce redundant instructions. (Transformation Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_IntstructionMoveBackward)** 

      --- DominatorTree algorithm, parallelism, redudant instruction removal

**Experiment 12(c)**: Since LLVM-provided loop strength reduction (LSR) pass limits itself according to the Target ISA addressing mode, an aggressive LSR pass is implemented for HLS to reduce the cost of DSPs on FPGA. (Transformation Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_AggressiveLSR_MUL)** 

      --- Loop Strength Reduction, Analysis based on Scalar Evolution, AddRecExpr, (Aggressive: may not stable for all the situations 0-0, but it works fine temporarily)
      --- Thank Momchil Velikov for his detailed reply about my inquiry about LSR pass

**Experiment 13**: Before involving HLS directives (array partitioning, loop pipelining/unroll), we need to check the memory access pattern of arrays in the source code (Analysis Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_ArrayAccessPattern)** 

      --- SCEV, Scalar Evolution Interpretation (AddRec, Add, Unknown, Constant), Array Access Pattern

**Experiment 13d**: unroll the specifc loops according to the label set in the source code and the setting in the configuration file (Transformation Pass) **[PASS CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_LoopUnroll)**  **[TEST CODE](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/LLVM_exp13d_HI_LoopUnroll)** 

      --- Loop Unrolling, Debug Information from Source Code

**Experiment 14**: (WIP) Based on the IR file, evaluate the resource and timing of the Window/Buffer/Filter in HLS 

      --- IR transformation, Timing/Resource Evaluation for special structures on FPGA
	
      


***

## [Further development](https://github.com/zslwyuan/Light-HLS#further-development)

If you want to do your own works based on this project, following hints might be useful.

1. user can add their passes according to the examples in the directory  **["Implementations"](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations)**. 
2. Note for development: (a) remember to define unique marco for header files (like #ifndef _HI_HI_POLLY_INFO);  (b) Modify the CMakelists.txt file in the 4 directories: **the pass directory([example](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/HI_SimpleTimingEvaluation/CMakeLists.txt)), Implementation directory([example](https://github.com/zslwyuan/Light-HLS/tree/master/Implementations/CMakeLists.txt)), LLVM_Learner_Libs directory([example](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/LLVM_Learner_Libs/CMakeLists.txt)) and the test directory([example](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/LLVM_exp7_DuplicateInstRemove/CMakeLists.txt))**. The modification should add subdirectory and consider the including path/library name.

## Good Good Study Day Day Up \(^o^)/~
