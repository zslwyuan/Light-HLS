# Light-HLS (LLVM v11): Fast, Accurate and Convenient 

*Let's try to make HLS developemnt easier for everyone~ ^\_^.*
     


Light-HLS is a light weight high-level synthesis (HLS) framework for academic exploration and evaluation, which can be called to perform various design space exploration (DSE) for FPGA-based HLS design. It covers the abilities of previous works, overcomes the existing limitations and brings more practical features. Light-HLS is modularized and portable so designers can use the components of Light-HLS to conduct various DSE procedures.  Light-HLS gets rid of RTL code generation so it will not suffer from the time-consuming synthesis of commercial HLS tools like VivadoHLS, which involves many detailed operations in both its frond-end and back-end, but can accurately estimate timing, resource and some other results of commercial tools for applications.

If Light-HLS helps for your works, please cite [our paper in ICCAD 2019](https://ieeexplore.ieee.org/document/8942136) ^_^: 

    T. Liang, J. Zhao, L. Feng, S. Sinha and W. Zhang, "Hi-ClockFlow: Multi-Clock Dataflow Automation and Throughput Optimization in High-Level Synthesis," 2019 IEEE/ACM International Conference on Computer-Aided Design (ICCAD), Westminster, CO, USA, 2019, pp. 1-6. doi: 10.1109/ICCAD45719.2019.8942136


**[A well-organzied Wiki can be find here](https://github.com/zslwyuan/Light-HLS/wiki)**. Since we are still developing this project and there could be some bugs and issues, if you have any problems, PLEASE feel free to let us know: ( tliang@connect.ust.hk ), for which we will sincerely appreciate ^\_^. We strongly recommand to you send us an email so we can add you into our maillist for the latest information of Light-HLS, because Light-HLS is a young tool and continuously updated to add features and fix bugs. This is a young project and if you want to join us, we are happy to make it a better one togather! \\^_^/ Here are some [known issues](https://github.com/zslwyuan/Light-HLS/wiki/Known-Issues) raised by our users, which we are handling one by one.



<img src="https://github.com/zslwyuan/Light-HLS/blob/master/Images/Light-HLS-Overview.png" width="800"> 

## Light-HLS Frond-End

The goal of Light-HLS frond-end is to generate IR code close enough to those generated via commercial tools, like VivadoHLS, for DSE purpose. In the front-end of Light-HLS, initial IR codes generated via Clang will be processed by HLS optimization passes consisted of three different levels: (a) At instruction level, Light-HLS modifies, removes or reorders the instructions, e.g. reducing bitwidth,  removing redundant instruction  and reordering computation. (b) At loop/function level, functions will be instantiated and loops may be extracted into sub-functions. (c) As for memory access level, redundant load/store instructions will be removed based on dependency analysis.

## Light-HLS Back-End

The back-end of Light-HLS is developed to schedule and bind for the optimized IR codes, so it can predict the resultant performance and resource cost accurately based on the given settings. The IR instructions can be automatically characterized by Light-HLS and a corresponding library, which records the timing and resource of different types of operations, will be generated. For scheduling, based on the generated library, Light-HLS maps most operations to corresponding cycles based on as-soon-as-possible (ASAP) strategy. For some pipelined loops, the constraints of the port number of the BRAMs and loop-carried dependencies are considered. Moreover, some operations might be scheduled as late as possible (ALAP) to lower the II. 	As for resource binding, Light-HLS accumulates the resource cost by each operation and the chaining of operations is considered. The reusing of hardware resource is an important feature in HLS and Light-HLS reuses resources based on more detailed rules, e.g. the source and type of input.

## Light-HLS Application Scenariors

Let's first see what we can do with Light-HLS in the research about HLS.

1. HLS designs can be set with various configurations, leading to different results of performance and resource. To find the optmial solution, designers can determine the configuration and call Light-HLS to predict the result in tens milliseconds, which will be close to the result in VivadoHLS. An example is **[Hi-ClockFlow](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/Hi_ClockFlow)**, a tool which searches for the configuration of clock settings and HLS directives for the multi-clock dataflow.

2. HLS designs are sensitive to the source codes, some of which are friendly to FPGA while the others are not. If researchers want to analyze and optimize the design at source code level, Light-HLS have accomplished the back-tracing from back-end, to front-end, to source code, so researchers can find out which part of source code have some interesting behaviors causing problems. In the example **[Hi-ClockFlow](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/Hi_ClockFlow)**, Light-HLS helps to partition the source code and map the performance and resource to the different parts of the source code.

3. In the front-end of HLS, source code will be processed by a series of LLVM Passes for analysis and optimization. However, for most of researchers, even if they come up with an idea for the front-end processing, they can hardly estimate the exact outcome of the solution if it can be applied to the commercial HLS tools. Currently, Light-HLS can generate IR code similar to the one generated by VivadoHLS and provide accurate scheduling and resource binding in back-end. Therefore, researchers might implement a Pass, plug it into the front-end of Light-HLS (Yes, just plug ^_^), and evaluate it with the back-end of Light-HLS to see the effect.

4. In the back-end of HLS, the IR instructions/blocks/loops/functions are scheduled and binded to specific hardware resource on FPGA. Based on the IR code similar to the one generated by commercial tools, how to properly schedule the source code and bind the resource can be tested and analyzed with Light-HLS. Currently, Light-HLS can provide the estimated performance and resource cost close to those from VivadoHLS 2018.2. (We will catch up the version of 2019.2 recently.) Light-HLS can generate the library of the timing and resource cost of all the IR instructions, e.g. add, fmul, MAC, fptoui and etc, for a specified devive, like Zedboard. Researchers can change the original scheme of Light-HLS's scheduling and binding to see the effect.


## Category:

**[Installation of Light-HLS](https://github.com/zslwyuan/Light-HLS/wiki/Installation-of-Light-HLS)**

**[Usage of Light-HLS](https://github.com/zslwyuan/Light-HLS/wiki/Usage-of-Light-HLS)**

**[Implementation of Light-HLS and Further development](https://github.com/zslwyuan/Light-HLS/wiki/Implementation-of-Light-HLS-and-Further-Development)**

**[Notes for You to Create Your Own HLS Tools with LLVM](https://github.com/zslwyuan/LLVM-9.0-Learner-Tutorial)**


## Good Good Study Day Day Up \(^o^)/~
