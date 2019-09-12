---
layout: page
title: 'E. Workflow Execution Fundamentals'
order: 500
usemathjax: true
---

#### Learning objectives:

  - Understand the fundamental steps involved when executing a workflow on a cyberinfrastructure that comprises storage and compute services

  - Be able to estimate the time it should take to complete a chain workflow on a simple cyberinfrastructure; 

  - Be able to analyze a (simulated) workflow execution timeline based on textual and graphical simulation output;
      
  - Understand the concept of data-intensive vs. compute-intensive workflows.

---


### Overview

Now that you have been introduced to *workflows*, *cyberinfrastructure*,
and *workflow management systems (WMS)*, you can start looking at actual
workflow executions.  Below, we first describe an example  scenario, which
includes a workflow, a platform, and a (trivial) WMS implementation. We
then detail the sequence of workflow execution events and formulate a
simple equation to model the expected execution time (or *makespan*).
Finally, we provide you with a simulator of workflow executions for this
scenario. You must run this simulator and answer a number of questions
based on simulation output!

### Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/
workflow_execution_fundamentals/workflow.svg">Workflow</object>

Figure 1 illustrates the DAG representation of the workflow for our
scenario. *task0* requires the file *task0::0.in* as its input and produces
the file *task0::0.out* as output. *task1* requires the output of *task0*
(file *task0::0.out*) as its input and produces the file *task1::0.out* as
output.  This type of linear workflow is often called a *chain*. In order
to orchestrate the execution of this workflow, a WMS needs access to two
types of resources: persistent storage to read/write files to/from and a
compute resource to perform the computation required by each task.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/
workflow_execution_fundamentals/platform.svg">Workflow</object>

The platform in Figure 2 depicts the cyberinfrastructure on which we can
execute this workflow. In this scenario, the WMS resides on the host
*my_lab_computer.edu*, and has access to both the Storage Service on host
*storage_db.edu* and the Compute Service on host *hpc.edu*.

**Storage Service**. A Storage Service (SS) stores files and handles read and write
requests. For example, if *my_lab_computer.edu* would like to read a file from
*storage_db.edu*, it will make a read request to the SS. Then the
file will be sent over the network from *storage_db.edu* to *my_lab_computer.edu*.
Say, for example, this file is 100 MB, and the link between the two hosts has a bandwidth of
10 MB/sec and a latency of 10 microseconds. The estimated amount of time it will take
to perform the *file read* operation can be estimated as follows:

$$
\begin{align}

  T_{read\:100MB\:file} & = \text{latency} + \frac{\text{file size}}{\text{bandwidth}} \\
                        & = 10\;\text{us} + \frac{100\;\text{MB}}{10\;\text{MB/sec}} \\
                        & = 10.000010\;\text{sec}\\
                        & \simeq 10\;\text{sec}

\end{align}
$$

**Compute Service**. A Compute Service (CS) can execute tasks submitted to it by the WMS.  Typically, a compute service will have access to faster hardware than
your typical machine and so it can execute workflow tasks faster.
For example, say we were to perform 100 TFlops of computation
on *my_lab_computer.edu* (which computes at speed 35 GFlop/sec, as seen in the figure). This is expected to take:

$$
\begin{align}

 T_{task0\:computation} & = \frac{100 * 10^{12}\;\text{flop}}{35 * 10^{9}\;\text{flop/sec}} \\
           & \simeq 2857.14\;\text{sec}

\end{align}
$$  

If instead we were to perform 100 TFlops of computation via the CS on
*hpc.edu* (which computes at speed 1000 GFlop/sec) , this computation is expected to take:

$$
\begin{align}

  T_{task0\:computation} & = \frac{100 \times 10^{12}\;\text{flop}}{10^{12}\;\text{flop/sec}} \\
                        & = 100\;\text{sec}

\end{align}
$$

The CS is able to compute 100 Tflops about *28* times faster than *my_lab_computer.edu*. In our scenario, the WMS only uses the CS on *hpc.edu* to execute
workflow tasks.


**Workflow Management System**. The WMS in this scenario greedily submits
tasks to the CS once they become ready. A task is ready to be submitted by the
WMS when it has been notified that the current task's parent task(s) has
completed. For example, the WMS can submit *task1* to the CS only once the CS notifies the WMS that *task0* has completed (i.e., the output of *task0* has been
produced and saved somewhere). In this example scenario, When the WMS submits a task to the CS, it
specifies that all file read and write operations be done through the Remote Storage
Service located at *storage_db.edu*. Additionally, the initial input file, *task0::0.in*,
is assumed to already be "staged" at the SS.  

### The Workflow Execution

Figure 3 below illustrates what happens during the execution of our workflow.
In order for the CS to complete each task in its entirety, it must read in the
input files for that task, perform the computation required by that task, and
finally write the output files for that task.  

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/
workflow_execution_fundamentals/workflow_execution.svg">Workflow Execution</object>

Notice that in step 3, the CS writes the output file to the SS, then immediately
reads that file back from the SS in step 4. This happens because file operations
are assigned to the SS at *storage_db.edu*, therefore file *task0::0.out*
simply cannot be "cached" by the CS for reuse (obviously this is not the best approach,
but that's what we are stuck with for now).

Using figure 3 as a guide and the two formulas stated above, we can estimate the
amount of time it would take to execute our workflow. The estimated execution time
of *task0* is as follows:

$$
\begin{align}

  T_{task0} & = T_{step1} + T_{step2} + T_{step3} \\
            & = (10\;\text{us} + \frac{200\;\text{MB}}{10\;\text{MB/sec}}) + (\frac{100*10^{12}\;\text{flop}}{10^{12}\;\text{flop/sec}}) + (10\;\text{us} + \frac{400\;\text{MB}}{10\;\text{MB/sec}}) \\
            & = 20.000010 + 100 + 40.000010\;text{sec} \\
            & = 160.000020\;\text{sec}\\
            & \simeq 160\;\text{sec}

\end{align}
$$

Next, the estimated execution time of *task1* is as follows:

$$
\begin{align}

  T_{task1} & = T_{step4} + T_{step5} + T_{step6} \\
            & = (10\;\text{us} + \frac{400\;\text{MB}}{10\;\text{MB/sec}}) + (\frac{35*10^{12}\;\text{flop}}{10^{12}\;\text{flop/sec}}) + (10\;\text{us} + \frac{100\;\text{MB}}{10\;\text{MB/sec}}) \\
            & = 40.000010 + 35 + 10.000010\;text{sec} \\
            & = 85.000020\;\text{sec}\\
            & \simeq 85\;\text{sec}

\end{align}
$$

*task0* and *task1* are executed sequentially therefore the total estimated
makespan of our workflow will be:

$$
\begin{align}

  T_{workflow\;makespan} & = T_{task0} + T_{task1} \\
                                & \simeq 160 + 85\;text{sec} \\
                                & = 245\;\text{sec}

\end{align}
$$



### Workflow Execution Simulation

So that you can gain hands-on experience with workflow executions, we provide ou with a
simulator for a scneario that ressembles the above example scenario. 

