---
layout: page
title: 'E. Workflow Execution Fundamentals'
order: 500
usemathjax: true
---

#### Learning objectives:

  - Understand the fundamental steps involved when executing a workflow on a cyberinfrastructure that comprises storage and compute services

  - Be able to analyze a (simulated) workflow execution timeline based on textual and graphical simulation output;
      
  - Be able to estimate the time it should take to complete a chain workflow on a simple cyberinfrastructure; 
      
  - Understand the concept of data-intensive vs. compute-intensive workflows.

---


### Overview

Now that you have been introduced to *workflows*, *workflow management systems (WMS)*, and 
*cyberinfrastructure*, we
proceed by taking a closer look into what actually happens when *workflows* (the applications)
are executed by a *WMS* (the software to manage application executions) on some *cyberinfrastructure* (the execution platform). First we describe a
scenario that provides a specific context for observing the execution of a workflow. This includes
the composition of the workflow, the platform specifications, and
finally the WMS implementation. Then we detail the sequence of workflow execution events and formulate a simple equation to model
the expected execution time (or *makespan*).


### Workflow Execution Simulation

WMS research and development is rife with challenges, many of which come from the difficulty to perform experiments (experiments are costly, time-consuming, not always repeatable, and/or limited to available hardware and software infrastructures). To address this difficult, the WRENCH project provides ways to simulate the execution of a WMS that runs a workflow on an arbitrary cyberinfrastructure configuration, in a way that is accurate, fast, and only requires a single standard laptop computer. In these pedagogic modules we use WRENCH for educational purposes.


### Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/
workflow_execution_fundamentals/workflow.svg">Workflow</object>

Figure 1 illustrates the DAG representation of the workflow for our
scenario. *task0* requires the file *task0::0.in* as its input and produces
the file *task0::0.out* as output. *task1* requires the output of *task0*
(file *task0::0.out*) as its input and produces the file *task1::0.out* as
output.  In order to orchestrate the execution of this workflow, a WMS
needs access to two types of resources: persistent storage to read/write
files to/from and a compute resource to perform the computation required by
each task.

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
Say, for example, this file is 100 MB, and the link between the two hosts has an effective bandwidth of
10 MB/sec and a latency of 10 microseconds. The estimated amount of time it will take
to perform the *file read* operation can be estimated as follows:

$$
\begin{align}

  T_{read\:100MB\:file} & = latency + \frac{file\;size}{bandwidth} \\
                        & = 10us + \frac{100\;MB}{10\;MB/sec} \\
                        & = 10.000010\;seconds\\
                        & \simeq 10\;seconds

\end{align}
$$

**Compute Service**. A Compute Service (CS) can execute tasks submitted to it by the WMS.  Typically, a compute service will have access to faster hardware than
your typical machine and so it can execute workflow tasks faster.
For example, say we were to perform 100 TFlops of computation
on *my_lab_computer.edu* (which computes at speed 35 GFlop/sec, as seen in the figure). This is expected to take:

$$
\begin{align}

 T_{task0\:computation} & = \frac{100 * 10^{12}\;flop}{35 * 10^{9}\;flop/sec} \\
           & \simeq 2857.14\;seconds

\end{align}
$$  

If instead we were to perform 100 TFlops of computation via the CS on
*hpc.edu* (which computes at speed 1000 GFlop/sec) , this computation is expected to take:

$$
\begin{align}

  T_{task0\:computation} & = \frac{100 * 10^{12}\;flop}{10^{12}\;flop/sec} \\
                        & = 100\;seconds

\end{align}
$$

The CS is able to compute 100 Tflops about *28* times faster than *my_lab_computer.edu*. In our scenario, the WMS only uses the CS on *hpc.edu* to execute
workflow tasks.


**Workflow Management System**. The WMS in this scenario greedily submits
tasks to the CS once they become ready. A task is ready to be submitted by the
WMS when it has been notified that the current task's parent task(s) has
been completed (including saving its output). For example, the WMS can submit *task1* to the CS only once the
CS notifies the WMS that *task0* has completed (i.e., the output of *task0* has been
produced and saved somewhere). When the WMS submits a task to the CS, it
specifies that all file read and write operations be done through the Remote Storage
Service located at *storage_db.edu*. Additionally, the initial input file, *task0::0.in*,
is assumed to already be "staged" at the SS.  

### The Workflow Execution

Figure 3 bellow illustrates what happens during the execution of our workflow.
In order for the CS to complete each task in its entirety, it must read in the
input files for that task, perform the computation required by that task, and
finally write the output files for that task.  

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/
workflow_execution_fundamentals/workflow_execution.svg">Workflow Execution</object>

Notice that in step 3, the CS writes the output file to the SS, then immediately
reads that file back from the SS in step 4. This happens because file operations
are assigned to the SS at *storage_db.edu*, therefore file *task0::0.out*
simply cannot be "cached" by the CS for reuse.

Using figure 3 as a guide and the two formulas stated above, we can estimate the
amount of time it would take to execute our workflow. The estimated execution time
of *task0* is as follows:

$$
\begin{align}

  T_{task0} & = T_{step1} + T_{step2} + T_{step3} \\
            & = (10us + \frac{200\;MB}{10\;MB/sec}) + (\frac{100*10^{12}\;flop}{10^{12}flop/sec}) + (10us + \frac{400\;MB}{10\;MB/sec}) \\
            & = 20.000010 + 100 + 40.000010 \\
            & = 160.000020\;seconds\\
            & \simeq 160\;seconds

\end{align}
$$

Next, the estimated execution time of *task1* is as follows:

$$
\begin{align}

  T_{task1} & = T_{step4} + T_{step5} + T_{step6} \\
            & = (10us + \frac{400\;MB}{10\;MB/sec}) + (\frac{35*10^{12}\;flop}{10^{12}flop/sec}) + (10us + \frac{100\;MB}{10\;MB/sec}) \\
            & = 40.000010 + 35 + 10.000010 \\
            & = 85.000020\;seconds\\
            & \simeq 85\;seconds

\end{align}
$$

*task0* and *task1* are executed sequentially therefore the total estimated
makespan of our workflow will be:

$$
\begin{align}

  T_{workflow\;makespan} & = T_{task0} + T_{task1} \\
                                & \simeq 160 + 85 \\
                                & = 245\;seconds

\end{align}
$$

### Conclusion

In this primer we have demonstrated what a simple workflow execution looks like
for a specific scenario. The scenario includes a workflow description,
platform specifications, and a WMS implementation. Based on these elements,
we were able to break down the execution of the workflow into steps,
estimate the duration of each step, and finally estimate the duration of the
entire workflow execution. The upcoming activities
will follow a similar format: a scenario will be presented and you
will evaluate certain aspects of the workflow execution. Continue on
to the next section,
[Activity 1: Running Your First Simulated Workflow Execution]({{ site.baseurl }}/activities/activity_1).
