# Preemptive-Cpu-Scheduling-in-C-

This small program focuses on process scheduling. Preemptive priority scheduling algorithm is used to schedule all processes, which involves context switching when a higher priority process enters the ready queue.

HOW THE SCHEDULER WORKS

Main data structure is the priority queue. Process arrival is simulated via a priority queue(it is called  a_pq in the source code). This priority queue simply stores processes according to their arrival time.  Other priority queue is the ready queue, which stores the processes according to their priorities.

Other than that we have one class and one struct, process and instruction respectively. Process class’s fields include information about the process like ID, priority, arrival time, finishing time, execution time, remaining instructions etc. Instruction struct’s fields include information about instruction’s ID and burst time.

Information in definition.txt is collected, processes and instructions are created according to it. Then all processes are loaded into a_pq. A while loop executes until this arrival priority queue is empty, and every process is terminated. An inner while loop executes until the ready queue is empty. Inner loop pops a process, executes its first instruction in it’s instruction queue and pushes it back to the ready queue, if the instruction ID is not exit. The outer loop checks the time and pops a process and pushes it to ready queue if the time has come. Of course, time variable is updated during this instructions.

HOW TO COMPILE

This scheduler is tested on Ubuntu 16.04 using g++ version 5.5.0 and C++11. C++11 is mandatory to be able compile the source code. To compile it simply use the following command : 
```
"g++ main.cpp"
```
Keep in mind that main.cpp, process.h.

HOW TO EXECUTE

To execute it use the following command : 
```
./a.out
```
Again keep in mind that a.out, definition.txt and code files should be in the same directory. Output.txt is also created in the same directory.

