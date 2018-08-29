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
Figure 1.2. A CS (CS) will execute tasks that the Workflow Management System (WMS)
submits to it. The Storage Service stores files, much like a database, and handles read and write requests. When the WMS submits a job to the CS, information is included in the
job submission that specifies what storage service to use for I/O operations.
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
above, `[0.000000][Tremblay:wms_activity1_3] About to execute a workflow with 5 tasks` tells us
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
    must perform *10 Tflop*. About how long should we expect *task4* to compute for?
  - Based on the simulation output, from the WMS's perspective, how long does it take
    for *task4* to complete?
  - Why does *task4* take longer than what you computed in *question 3*?
  - Assuming there is no traffic on the network, about how long would it take to send all of
    *task4*'s input data from *Fafard* to *Jupiter* and to send all of *task4*'s output data
    from *Jupiter* to *Fafard*, using the direct link between these two hosts and assuming no other
    network traffic?
  - Accounting for this I/O *overhead*, does *task4*'s execution time as experienced by the WMS make sense?

{% comment %}

1. task1 was submitted at 201.550215

2. about 10s (10TF/1TF)

3. task4 submit at 428.61, task4 said to be complete at 505.73, so 505.73 - 428.61 = 77.12s

4. task4 task longer because first, 3 files totaling 520MB must be read in from SS, then 10s of compute, then 1 100MB file must be written to SS

5. (120 + 200 + 200 + 100) / 10 = 62 plus a little more for latency..

6. yes, expected read input time + compute time + expected write output time comes out to a little less than the execution time
    as experienced by the WMS

{% endcomment %}

## Step #3: Visualize the Workflow Execution

Analyzing the simulation output can be tedious, especially when the workflow comprises
a large number of tasks or when there are a number of complex software services being
simulated. Fortunately, the simulator can produce a visualization of the workflow execution
as a Gantt chart.

In the terminal run the following commands:
1. run `docker pull wrenchproject/wrench-pedagogic-modules:activity-1-visualization`
2. then run `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:activity-1-visualization`
3. open a browser and go to localhost:3000

**Questions**
  - What fraction of *task0*'s execution time is spent doing I/O?
  - What fraction of *task4*'s execution time is spent doing I/O?
  - If the link bandwidth between *Fafard* and *Jupiter* were doubled, what should
    the fraction of *task4*'s execution time is spent doing I/O be?
  - Double the platform link bandwidth (set it to 20MBps) using the visualization and run the simulation.
      Is your expectation confirmed?
  - With the link bandwidth doubled, how much faster is the workflow execution now than before?
  - What link bandwidth would be necessary for the workflow to run 2x faster
    than with the original 10MBps bandwidth? You can do this by solving a simple equation, and
     then check that your answer is correct using the simulation.

{% comment %}

1. looking at the chart, read input = 75.77, write output = 75.77, entire task = 201.54,
    so  (75.77 + 75.77) / 201.54 = 0.752 or about 75% of the time spent doing I/O

2. looking at the chart, read input = 56.29, write output = 10.82, entire task = 77.11
    so (56.29 + 10.82) / 77.11 = 0.87 or about 87% of the time spent doing I/O

3. read file time should be (120 + 200 + 200 + 100) / 20 = 31 + a little more for latency
    so 31 / (31 + 10) = 0.76 or about 76% of the time spent doing I/O

4. with the link bandwidth doubled in the visualization, we have task4 read input = 28.14, write output = 5.4
    and entire task = 43.55 so about (28.14 + 5.4) / 43.55 = 0.77 or about 77% of the time spent doing I/O

5. with the link bandwidth at 10MBps and a single remote storage service, execution time T = 505.73. we want T/2 or 252.86 seconds
    so looking at the chart, i solve for B in 252.86 = (1/B)sum(read inputs) + compute times + (1/B)sum(write outputs)
    B = 3.6 and so the bandwidth should be increased to 36MBps to get a workflow execution twice as fast.. using the
    simulation, I can confirm that this is true

{% endcomment %}

## Step #4: Better Data locality with another storage service on the compute host

The CS is reading and writing files from and to a remote storage service, thus contributing
to I/O overhead. This overhead can be reduced if the storage service were located on the same host as
the CS. Here we introduce the idea of moving computations closer to where the
data resides, or **data locality**.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/scenario_2.svg">Scenario 2</object>


In the previous steps we ran simulations using the platform scenario described in Figure 1.2 where the compute
service accessed a *remote storage service* to perform all of the read and write operations as required by the workflow in
Figure 1.1. Now, consider the scenario where a WMS user is only concerned with accessing the initial input file, *task0::0.in* and
the final output file, *task4::0.out* via the remote storage service. Other files created during the execution of the workflow need
not be analyzed or accessed by the WMS user and serve only as intermediate steps required to complete the workflow in its entirety.
Furthermore, let us say that another storage service resides on the host Jupiter and that the CS has access to this
storage service. Since the WMS user will only access the *remote storage service* to handle the two files, *task0::0.in* and *task4::0.out*,
the WMS is implemented such that it requests the CS to use its *local storage service* (the storage service located on Jupiter) for all read and write operations.
Figure 1.3 above illustrates our new cyber infrastructure and WMS/Workflow scenario.

Using the visualization tool from Step 3, input *10MBps* as the link bandwidth.
Select the radio button that says: *Storage Service on Fafard and Jupiter*. Run the simulation.

**Questions**
  - What fraction of *task4* is spent doing I/O?
  - How much faster is the workflow execution now than compared to what was observed in *Step 2*?
  - Using only a single remote storage service, what would you need to increase the bandwidth to in order to have a workflow execution that is
    faster than what was realized using a 10MBps link bandwidth with a storage service on Fafard and Jupiter?
  - Other than affecting the workflow execution, what is another benefit of utilizing data locality?

{% comment %}

1. looking at the chart, read input = 1.07, write output = 10.82, entire task = 21.9 so (1.07 + 10.82) / 21.9 = 0.54 or
    about 54% of task4 is spent doing I/O. what we saw in step 3 question 2 was that with the remote storage service,
    87% of task4 was spent doing I/O

2. (505.73 - 246.66) / 505.73 = 0.5122 or about 51.22% faster

3. using the same formula from step 3 number 5, i solve for B in (T with local storage) < (1/B)sum(read inputs) + compute times + (1/B)sum(write outputs)
  B = 3.85 so bandwidth should be roughly 39MBps or greater

4. that link could be freed for use by others. the system could have a higher throughput if more operations using that link were to be performed
    while our CS jobs are using its local storage service

{% endcomment %}

When you are finished using the visualization tool, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:activity-1-visualization)`

# Conclusion

In this activity, we have simulated the execution of a small workflow on two simple distributed computing environments that exposed the cost of performing I/O operations.
Using both textual and visualized simulation output, we have become familiarized with network bandwidth and **data locality**, two of the many factors that can affect task **turnaround** times, which consequently affect overall workflow execution performance. With these concepts in mind, proceed to the next
 activity, "Activity 2: Parallelism", where we construct a more complex distributed computing environment in order to explore the concept of "parallelism", its benefits,
 and limitations.
