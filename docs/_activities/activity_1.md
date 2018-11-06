---
layout: page
title: 'Activity 1: Running Your First Simulated Workflow Execution'
order: 2
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
  - Understand I/O overhead effects on workflow executions;
  - Gain exposure to the concept of data locality and its effect on workflow execution.


# Overview

## Workflow and Platform Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/workflow.svg">Workflow</object>

In this activity, we study the execution of the workflow depicted in Figure 1 on the cyber infrastructure depicted in
Figure 2. A Compute Service (CS) will execute tasks that the Workflow Management System (WMS)
submits to it. The CS has at its disposal a single core and will execute only one task at a time.
The Storage Service (SS) stores files, much like a database, and handles read and write requests. When the WMS submits a job to the CS, information is included in the
job submission that specifies what storage service to use for I/O operations.
This is a very simple scenario, and will be used to get our "feet wet" with WRENCH simulations.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/cyber_infrastructure.svg">Cyber Infrastructure</object>

## WMS Scenario

We execute the workflow with a (already implemented) WMS that executes tasks
on the CS as soon as possible. Each task running on the CS reads and writes data
from/to the SS (which, from the perspective of the task, is on a remote host).
Once the WMS is notified by the CS that a task has completed, it will greedily submit the next ready task
going from left-to-right in the depicted workflow whenever multiple tasks are ready.
For example, when *task0* has completed and its output has been written to the SS,
*task1*, *task2*, and *task3* become ready. At this point, the WMS will submit *task1* for
execution to the CS.
This process is repeated until workflow execution is complete.

# Activity

## Step #1: Run the Simulation

In a terminal, run the following commands.

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-1`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-1`

Step 2 will display textual simulation output to your terminal window. This output indicates
(simulated) actions and events throughout the execution of the workflow.

## Step #2: Interpret the Workflow Execution

<div class="wrench-output">
<span style="font-weight:bold;color:rgb(187,0,187)">[0.000000][my_lab_computer.edu:wms_activity1_3] Starting on host my_lab_computer.edu listening on mailbox_name wms_activity1_3<br></span>
<span style="font-weight:bold;color:rgb(187,0,187)">[0.000000][my_lab_computer.edu:wms_activity1_3] About to execute a workflow with 5 tasks<br></span>
<span style="font-weight:bold;color:rgb(0,0,187)">[0.000000][my_lab_computer.edu:wms_activity1_3] Submitting task0 as a job to compute service on hpc.edu<br></span>
<span style="font-weight:bold;color:rgb(187,0,0)">[33814.005184][my_lab_computer.edu:wms_activity1_3] Notified that task0 has completed<br></span>
<span style="font-weight:bold;color:rgb(0,0,187)">[33814.005184][my_lab_computer.edu:wms_activity1_3] Submitting task1 as a job to compute service on hpc.edu<br></span>
<span>.<br></span>
<span>.<br></span>
<span>.<br></span>
</div>

The simulation will produce output similar to the above snippet of text. The first column denotes the simulation time at which some process is performing some action.
The second column is split into two sections: hostname, and process name. Last is a message describing what the process is doing. For example, the second line from the output
above, `[0.000000][my_lab_computer.edu:wms_activity1_3] About to execute a workflow with 5 tasks` tells us
that at *simulation time 0.00000*, the WMS named *wms_activity1*, located on the physical host, *my_lab_computer.edu*, is *"About to execute a workflow with 5 tasks"*.
Note that the process name is actually *wms_activity1_3*. The "3" there is added to distinguish different instances of the WMS in case
the simulation executes multiple of them (which we don't do in this activity).
Simulation output for this activity has been constrained such that only messages from the WMS are visible. Furthermore, the color scheme of the
output has been set up such that general messages are <span style="font-weight:bold;color:rgb(187,0,187)">pink</span>, task submissions to the CS are
<span style="font-weight:bold;color:rgb(0,0,187)">blue</span>, and notifications received from the CS are
<span style="font-weight:bold;color:rgb(187,0,0)">red</span>.  You'll note that, for instance, we do not see any simulation output
corresponding to what the SS is doing. In the following activities, we will expose more simulation output to highlight
areas of interest.

**Answer these questions**
  - [q1] At what time did the WMS submit *task1* as a job to the compute service?
  - [q2] From the WMS's perspective, how long did *task1* run for?
    (this duration is called the task's **turnaround-time**)
  - [q3] The compute service runs on a host with a speed of *1000 GFlop/sec*, and *task4*
    must perform *10 TFlop*. About how long should we expect *task4* to compute for?
  - [q4] Based on the simulation output, from the WMS's perspective how long does it take
    for *task4* to complete?
  - [q5] Why does *task4* take longer than what you computed in *question 3*?
  - [q6] Assuming there is no traffic on the network, about how long would it take to send all of
    *task4*'s input data from *storage_db.edu* to *hpc.edu* and to send all of *task4*'s output data
    from *hpc.edu* to *storage_db.edu*, using the direct link between these two hosts and assuming no other
    network traffic?
  - [p7] Accounting for this I/O *overhead*, does *task4*'s execution time as experienced by the WMS make sense?

{% comment %}

q1. task1 was submitted at 201.550215

q2. about 10s (10TF/1TF)

q3. task4 submit at 428.61, task4 said to be complete at 505.73, so 505.73 - 428.61 = 77.12s

q4. task4 task longer because first, 3 files totaling 520MB must be read in from SS, then 10s of compute, then 1 100MB file must be written to SS

q5. (120 + 200 + 200 + 100) / 10 = 62 plus a little more for latency..

q6. yes, expected read input time + compute time + expected write output time comes out to a little less than the execution time
    as experienced by the WMS

{% endcomment %}

## Step #3: Visualize the Workflow Execution

Analyzing the textual simulation output can be tedious, especially when the workflow comprises
many tasks and/or when there are many simulated software services. Fortunately, the simulator can produce a visualization of the workflow execution
as a Gantt chart.

In the terminal run the following commands:
1. run `docker pull wrenchproject/wrench-pedagogic-modules:activity-visualization`
2. then run `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:activity-visualization`
3. open a browser and go to [localhost:3000/activity_1](localhost:3000/activity_1)

**Answer these questions**
  - [q7] What fraction of *task0*'s execution time is spent doing I/O?
  - [q8] What fraction of *task4*'s execution time is spent doing I/O?
  - [q9] If the link bandwidth between *storage_db.edu* and *hpc.edu* were doubled, what should
    the fraction of *task4*'s execution time is spent doing I/O be?
   Double the platform link bandwidth (set it to 20MBps) using the visualization and run the simulation.
      Is your expectation confirmed?
  - [q10] With the link bandwidth doubled, how much faster is the workflow execution now than before?
  - [q11] What link bandwidth would be necessary for the workflow to run 2x faster
    than with the original 10MBps bandwidth? Hint: You can do this by solving a simple equation, and
     then check that your answer is correct using the simulation.

{% comment %}

q7. looking at the chart, read input = 75.77, write output = 75.77, entire task = 201.54,
    so  (75.77 + 75.77) / 201.54 = 0.752 or about 75% of the time spent doing I/O

q8. looking at the chart, read input = 56.29, write output = 10.82, entire task = 77.11
    so (56.29 + 10.82) / 77.11 = 0.87 or about 87% of the time spent doing I/O

q9. read file time should be (120 + 200 + 200 + 100) / 20 = 31 + a little more for latency
    so 31 / (31 + 10) = 0.76 or about 76% of the time spent doing I/O

q10. with the link bandwidth doubled in the visualization, we have task4 read input = 28.14, write output = 5.4
    and entire task = 43.55 so about (28.14 + 5.4) / 43.55 = 0.77 or about 77% of the time spent doing I/O

q11. with the link bandwidth at 10MBps and a single remote storage service, execution time T = 505.73. we want T/2 or 252.86 seconds
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


In the previous steps we ran simulations using the platform scenario described in Figure 2 where the compute
service accessed a *remote storage service* to perform all of the read and write operations as required by the workflow in
Figure 1. Now, consider the scenario where a WMS user is only concerned with accessing the initial input file, *task0::0.in* and
the final output file, *task4::0.out* via the remote storage service. Other files created during the execution of the workflow need
not be analyzed or accessed by the WMS user and serve only as intermediate steps required to complete the workflow in its entirety.
Furthermore, let us say that another storage service resides on the host hpc.edu and that the CS has access to this
storage service. Since the WMS user will only access the *remote storage service* to handle  two files, *task0::0.in* and *task4::0.out*,
we have enhanced our previous WMS implementation so that it tells the CS to use its *local storage service* (the storage service located on hpc.edu) for all
read and write operations for intermediate files.
Figure 3 above illustrates our new cyber infrastructure and WMS/Workflow scenario.

Using the visualization tool from Step 3, input *10MBps* as the link bandwidth.
Select the radio button that says: *Storage Service on storage_db.edu and hpc.edu*. Run the simulation.

**Answer these questions**
  - [q12] What fraction of *task4* is spent doing I/O?
  - [q13] How much faster is the workflow execution now than compared to what was observed in *Step 2*?
  - [q14] Using only a single remote storage service, what would you need to increase the bandwidth to in order to have a workflow execution that is
    faster than what was realized using a 10MBps link bandwidth with a storage service on storage_db.edu and hpc.edu?
  - [q15] Other than affecting the workflow execution, what is another benefit of utilizing data locality?

{% comment %}

q12. looking at the chart, read input = 1.07, write output = 10.82, entire task = 21.9 so (1.07 + 10.82) / 21.9 = 0.54 or
    about 54% of task4 is spent doing I/O. what we saw in step 3 question 2 was that with the remote storage service,
    87% of task4 was spent doing I/O

q13. (505.73 - 246.66) / 505.73 = 0.5122 or about 51.22% faster

q14. using the same formula from step 3 number 5, i solve for B in (T with local storage) < (1/B)sum(read inputs) + compute times + (1/B)sum(write outputs)
  B = 3.85 so bandwidth should be roughly 39MBps or greater

q15. that link could be freed for use by others. the system could have a higher throughput if more operations using that link were to be performed
    while our CS jobs are using its local storage service

{% endcomment %}

When you are finished using the visualization tool, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:activity-visualization)`

# Conclusion

In this activity, we have simulated the execution of a small workflow on two simple distributed computing environments that exposed the
cost of performing I/O operations.
Using both textual and visualized simulation output, we have familiarized ourselves with network bandwidth and **data locality**,
two of the many factors that can affect task **turnaround** times, which consequently affect overall workflow execution performance.
With these concepts in mind, proceed to the next
 activity, [Activity 2: Parallelism]({{ site.baseurl }}/activities/activity_2), where we construct a more complex distributed computing environment in order to explore the concept of "parallelism".
