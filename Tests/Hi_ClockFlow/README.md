This example of Light-HLS is **[Hi-ClockFlow](https://github.com/zslwyuan/Hi-ClockFlow/)**, to show how to implement a design space exploration with **[Light-HLS](https://github.com/zslwyuan/Light-HLS)**, detailed manual for which can be found **[here](https://github.com/zslwyuan/Light-HLS/wiki)**

Here, We mainly consider the parameters of clock frequencies of each module, array partitioning, loop pipelining and loop unrolling.

build Hi-ClockFlow with the following command:

       ./Build.sh

run with the command in the directory "build": 

       ./Hi_ClockFlow  <C/C++ FILE> <top_function_name>  <configuration_file> [DEBUG]


# Hi-ClockFlow: Multi-Clock Dataflow Automation and Throughput Optimization in High-Level Synthesis

Tools of high-level synthesis (HLS) are developed to improve the accessibility of FPGAs by allowing designer to describe hardware designs in high-level language, e.g. C/C++. However, the source codes of general applications are not structured as canonical dataflow. Furthermore, clock frequencies are powerful parameters to improve dataflow throughput but currently commercial HLS tools limit themselves to single clock domain. Consequently, in order to benefit from the multiple-clock dataflow design, designers still suffer from manually analyzing the applications, partitioning the source code into modules, optimizing them with appropriate parameters and resource allocation, and finally interconnecting them.  

We analyze the impact of multiple clock domains for HLS designs and present Hi-ClockFlow, an automatic HLS framework. Hi-ClockFlow, which is implemented **[here](https://github.com/zslwyuan/Light-HLS/tree/master/Tests/Hi_ClockFlow)** as one of Light-HLS applications, can analyze the source code based on **[Light-HLS](https://github.com/zslwyuan/Light-HLS)**, our light weight HLS evaluation framework, explore the large design space, and optimize such parameters as clock frequencies and HLS directives in dataflow. By properly partitioning the source code of an application into parts with various clock domains,   Hi-ClockFlow can optimize the dataflow with imbalanced modules and speed up the performance under the specific constraint of resource.


If Hi-ClockFlow helps for your works, please cite [our paper in ICCAD 2019](https://ieeexplore.ieee.org/document/8942136) ^_^: 

    T. Liang, J. Zhao, L. Feng, S. Sinha and W. Zhang, "Hi-ClockFlow: Multi-Clock Dataflow Automation and Throughput Optimization in High-Level Synthesis," 2019 IEEE/ACM International Conference on Computer-Aided Design (ICCAD), Westminster, CO, USA, 2019, pp. 1-6. doi: 10.1109/ICCAD45719.2019.8942136



<img src="https://github.com/zslwyuan/Light-HLS/blob/master/Images/hi-clockflow-overview.png" width="800"> 


***

## 1. Multi-Clock Dataflow DSE formulation

In the design space, the application is described as dataflow, which consists of multiple modules. Each module can has its own setting of HLS directives, i.e. parallelism, and clock frequency. Hi-ClockFlow will find the proper setting for each module to maximize the overall throughput of the application while meeting the resource constraint of the target FPGA device and the number constraint of clock domains.

The problem formulation is shown below:

<img src="https://github.com/zslwyuan/Light-HLS/blob/master/Images/hi-clockflow-formulation.png" width="800"> 


***

## 2. Pushing-Relaxation Heuristic Algorithm 

Hi-ClockFlow solves this problem based on pushing-relaxation heuristic algorithm.

<img src="https://github.com/zslwyuan/Light-HLS/blob/master/Images/pushrelax_fig.png" width="800"> 
<img src="https://github.com/zslwyuan/Light-HLS/blob/master/Images/pushrelax_code.png" width="400"> 

The detailed codes are provided in **[here](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/)**
Compared to common usage of Light-HLS, in the configuration file of Hi-ClockFlow, as **[an example](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/convs_settings/conv_config.txt)**, users need to specify the resource constraints and the initial clock for the modules in dataflow. The trace of this heuristic algorithm will be recorded in a log file "pushing\_relaxtion_log.txt".

The main flow of Hi-ClockFlow is implememnted in [Hi_ClockFlow.cc](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc). After [pre-processing of source code](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L45) and [analysis of the IR code](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L76-#100), Hi-ClockFlow begins [its PUSH-RELAX flow](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L133-#404), where it tries to PUSH the HLS directives and when PUSH trial failed, it will update the clock combination by RELAX. Please note that these decision is made by these [lines](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/Hi_ClockFlow.cc#L262-#402).

The **PUSH** operation is implemented with function [tryUpdateHLSDirectives()](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/ConfigGen.cc#L1025-L1292), in which Hi-ClockFlow find next combination of directives, including loop unrolling, loop pipelining and array partitioning, for the design to optimize the multi-clock dataflow performance. 

The **RELAX** operation is implemented with function [justTryUpdateSlowestFuncClock_withoutHLSDSE()](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/ConfigGen.cc#L1384-L1474), in which Hi-ClockFlow uses function [findNextPossibleClockCombination()](https://github.com/zslwyuan/Light-HLS/blob/master/Tests/Hi_ClockFlow/ConfigGen.cc#L1476-L1584) to get next combination of clocks for the dataflow design.

We will keep updating the strategy of this heuristic algorithm for the setting of directives and clocks to achieve better performance.

***

## 3. Hardware Implemenation of Multi-Clock Dataflow

According to the information from Hi-ClockFlow, designers can implement multi-clock dataflow for various applications.
The example projects of multi-clock dataflow are provided via **[Google Drive](https://drive.google.com/drive/folders/1WC4ndj2plVBTll_GDR3XtHzC8KRjG0NG?usp=sharing)** since we go through the synthesis, placement and routing for easier evaluation.
The example projects are implemented for Zedboard, a SoC evaluation platform. You can launch the application in VivadoSDK, check the run time and validate the results.

