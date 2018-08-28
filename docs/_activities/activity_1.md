---
layout: page
title: 'Activity 1: Running Your First Simulated Workflow Execution'
---

1. [Learning objectives](#learning-objectives)
2. [Overview](#overview)
3. [Activity](#activity)
4. [Conclusion](#conclusion)


# Learning Objectives

  - Understand a simple distributed workflow execution scenario;
  - Be able to analyze a (simulated) workflow execution timeline based on textual
    and graphical simulation output;
  - Be able to estimate the time it should take to complete a workflow task on a
    given compute host, accounting for I/O overhead;
  - Understand I/O overhead effects on workflow executions by changing simulation parameters;
  - Gain exposure to the concept of data locality and witness how it can reduce workflow execution time.


# Overview

## Workflow and Platform Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/workflow.svg">Workflow</object>

In this activity, we study the execution of the workflow depicted in Figure 1.1 on the cyber infrastructure depicted in
Figure 1.2. A Compute Service (CS) will execute tasks that the Workflow Management System
submits to it. The Storage Service stores files, much like a database, and handles read and write requests. When the WMS submits a job to the CS, information is included in the
job submission that specifies what storage service to use.
This is a very simple scenario, and will be used to get our "feet wet" with WRENCH simulations.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/cyber_infrastructure.svg">Cyber Infrastructure</object>

## WMS Scenario

We execute the workflow with a (already implemented) WMS that greedily submits tasks
to the Compute Service as soon as they become ready. Each task running on the CS reads and writes data
from/to the Storage Service (which, from the perspective of the task is on a remote host).
Once the WMS is notified by the CS that a task has completed, it will greedily submit the next ready task.
For example, when *task0* has completed and its output has been written to the storage service,
*task1*, *task2*, and *task3* become ready. At this point, the WMS will submit *task1* regardless of
any information it knows about *task2* or *task3*.
The process of greedily submitting tasks to the CS as they become ready is repeated until
the entire workflow is complete.

# Activity

## Step #1: Run the Simulation

In a terminal, run the following commands.

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-1`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-1`

Step 2 will display textual simulation output to your terminal window. This output indicates
(simulated) actions and events throughout the execution of the workflow.

## Step #2: Interpret the Workflow Execution

<div class="wrench-output">
<span style="font-weight:bold;color:rgb(187,0,187)">[0.000000][Tremblay:wms_activity1_3] Starting on host Tremblay listening on mailbox_name wms_activity1_3<br></span>
<span style="font-weight:bold;color:rgb(187,0,187)">[0.000000][Tremblay:wms_activity1_3] About to execute a workflow with 5 tasks<br></span>
<span style="font-weight:bold;color:rgb(0,0,187)">[0.000000][Tremblay:wms_activity1_3] Submitting task0 as a job to compute service on Jupiter<br></span>
<span style="font-weight:bold;color:rgb(187,0,0)">[33814.005184][Tremblay:wms_activity1_3] Notified that task0 has completed<br></span>
<span style="font-weight:bold;color:rgb(0,0,187)">[33814.005184][Tremblay:wms_activity1_3] Submitting task1 as a job to compute service on Jupiter<br></span>
<span>.<br></span>
<span>.<br></span>
<span>.<br></span>
</div>

The simulation will produce output similar to the above snippet of text. The first column denotes the simulation time at which some process is performing some action.
The second column is split into two sections: hostname, and process name. Last is a message describing what the process is doing. For example, the second line from the output
above, `[0.000000][Tremblay:wms_activity1_3(1)] About to execute a workflow with 5 tasks` tells us
that at *simulation time 0.00000*, the WMS named *activity1*, located on the physical host, *Tremblay*, is *"About to execute a workflow with 5 tasks"*.
Simulation output for this activity has been constrained such that only messages from the WMS are visible. Furthermore, the color scheme of the
output has been set up such that general messages are <span style="font-weight:bold;color:rgb(187,0,187)">pink</span>, task submissions to the CS are
<span style="font-weight:bold;color:rgb(0,0,187)">blue</span>, and notifications received from the CS are
<span style="font-weight:bold;color:rgb(187,0,0)">red</span>. In the following activities, we will expose more simulation output to highlight
areas of interest.

**Questions**
  - At what time did the WMS submit *task1* as a job to the compute service?
  - From the WMS's perspective, how long did *task1* run for?
    (this duration is called the task's **turnaround-time**)
  - The compute service runs on a host with a speed of *1000 GFlop/sec*, and *task4*
    must perform *100 Tflop*. About how long should we expect *task4* to compute for?
  - Based on the simulation output, from the WMS's perspective, how long does it take
    for *task4* to complete?
  - Why does *task4* take longer than what you computed in *question 3*?
  - Assuming there is no traffic on the network, about how long would it take to send all of
    *task4*'s input data from *Fafard* to *Jupiter* and to send all of *task4*'s output data
    from *Jupiter* to *Fafard*, using the direct link between these two hosts and assuming no other
    network traffic?
  - Accounting for this I/O *overhead*, does *task4*'s execution time as experienced by the WMS make sense?


## Step #3: Visualize the Workflow Execution

Analyzing the simulation output can be tedious, especially when the workflow comprises
a large number of tasks or when there are a number of complex software services being
simulated. Fortunately, the simulator can produce a visualization of the workflow execution
as a Gantt chart.

In the terminal run the following commands:
1. probably will be something like `docker pull wrenchproject/wrench-pedagogic-modules:activity-1-visualization`
2. then `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:activity-1-visualization`
3. open a browser and go to localhost:3000

**Questions**
  - What fraction of *task0*'s execution time is spent doing I/O?
  - What fraction of *task4*'s execution time is spent doing I/O?
  - If the link bandwidth between *Fafard* and *Jupiter* were doubled, what should
    the fraction of *task4*'s execution time is spent doing I/O be?
  - Double that link's bandwidth using the XXX. Is your expectation confirmed?
  - With the link bandwidth doubled, how much faster is the workflow execution now than before?
  - What link banwidth would be necessary for the workflow to run XXX faster
    than with the original 125MBps bandwidth? You can do this by solving a simple equation, and
     then check that your answer is correct using the simulation.

## Step #4: Better Data locality with another storage service on the compute host

The compute service is reading and writing files from and to a remote storage service, thus contributing
to I/O overhead. This overhead can be reduced if the storage service were located on the same host as
the compute service. Here we introduce the idea of moving computations closer to where the
data resides, or **data locality**.

In the previous steps we ran simulations using the platform scenario described in Figure 1.2 where the compute
service accessed a *remote storage service* to perform all of the read and write operations as required by the workflow in
Figure 1.1. Now, consider the scenario where a WMS user is only concerned with accessing the initial input file, *task0::0.in* and
the final output file, *task4::0.out* via the remote storage service. Other files created during the execution of the workflow need
not be analyzed or accessed by the WMS user and serve only as intermediate steps required to complete the workflow in its entirety.
Furthermore, let us say that another storage service resides on the host Jupiter and that the compute service has access to this
storage service. Since the WMS user will only access the *remote storage service* to handle files, *task0::0.in* and *task4::0.out*,
the WMS is implemented such that it requests the compute service to use its *local storage service* for all read and write operations.
Figure 1.3 illustrates our new cyber infrastructure. (possibly add Figure 1.4 which illustrates in detail what files will be read/written to
  which storage services)

Using the visualization tool from Step 3, input *10MBps* as the link bandwidth.
Select the radio button that says: *Storage Service on Fafard and Jupiter*. Run the simulation.

**Questions**
  - What fraction of *task4* is spent doing I/O?
  - How much faster is the workflow execution now than compared to what was observed in *Step 2*?
  - Using only a single remote storage service, what would you need to increase the bandwidth to in order to have a workflow execution that is
    faster than what was realized using a 10MBps link bandwidth with a storage service on Fafard and Jupiter? (looks like about 39 MBps)
  - Other than affecting the workflow execution, what is another benefit of utilizing data locality? (i'm thinking this will free up
    the network link so that it can be used for other things and possibly promote a higher throughput of the entire system if other
    operations were to be done that use that link)   

When you are finished using the visualization tool, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:activity-1-visualization)`

# Conclusion

In this activity, we have simulated the execution of a small workflow on two simple distributed computing environments. Using both textual and visualized simulation
output, we have become familiarized with network bandwidth and **data locality**, two of the many factors that can affect task **turnaround** times, which consequently affect
overall workflow execution performance. Distributed computing environments, however, are not limited to several single core machines and these days it is commonplace for such
environments to consist of multiple multi-core machines. Proceed to the next activity, "Activity 2: Parallelism", where we examine the concept of parallelism, its benefits,
and limitations.  
