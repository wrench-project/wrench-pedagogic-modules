---
layout: page
title: 'F. Workflows and Data Locality'
order: 600
---

1. [Learning objectives](#learning-objectives)
2. [Overview](#overview)
3. [Activity](#activity)
4. [Conclusion](#conclusion)


### Learning Objectives

  - Understand I/O overhead effects on workflow executions;
  - Gain exposure to the concept of data locality and its effect on workflow execution.


#### Workflow and Platform Scenario

In this activity, we study the execution of the workflow depicted in Figure 1 below. 

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/workflow.svg">Workflow</object>

We wish to execute this workflow on the cyberinfrastructure depicted in
Figure 2 below (which is the same as that used in the previous module). To
summarize, a Compute Service (CS) at host `hpc.edu` has a single core that
can execute workflow tasks (but only one at a time). Workflow files are
stored on the Storage Service (SS) at host `storade_bd.edu` that handles
read and write requests.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/cyber_infrastructure.svg">Cyberinfrastructure</object>

Like in the previous scenario, the WMS executes tasks as soon as they are
ready, so that each task runs on the CS and reads/writes all files on the
SS. Whenever multiple tasks are ready at the same time, 
as will be the case after *task0* has completed, 
the WMS arbitrarily runs them in lexicographical order (i.e., 
*task1*, then *task2*, and then *task3*). 


In the terminal run the following commands:
1. run `docker pull wrenchproject/wrench-pedagogic-modules:ics332-activity-visualization`
2. then run `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:ics332-activity-visualization`
3. open a browser and go to [localhost:3000/](localhost:3000/)
4. sign in using your `<UH Username>@hawaii.edu` Google Account
5. select `Workflow Execution: Data Locality`


### Data Locality

h another storage service on the compute host

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
Figure 3 above illustrates our new cyberinfrastructure and WMS/Workflow scenario.

Using the visualization tool from Step 3, input *10 MB/sec* as the link bandwidth.
Select the radio button that says: *Storage Service on storage_db.edu and hpc.edu*. Run the simulation.

**Answer these questions**
  - [q13] What fraction of *task4* is spent doing I/O?
  - [q14] Compared to the workflow execution time observed in *Step 2*, how much faster is it now?
  - [q15] Using only the remote storage service (i.e., no storage service close to the compute service), what would you need to increase the bandwidth to in order to have a workflow execution that is
    faster than what was realized using a 10 MB/sec link bandwidth with a storage service on storage_db.edu and hpc.edu?

When you are finished using the visualization tool, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:ics332-activity-visualization)`

# Conclusion

In this activity, we have simulated the execution of a small workflow on two simple distributed computing environments that exposed the
cost of performing I/O operations.
Using both textual and visual simulation output, you have familiarized yourselves with network bandwidth and **data locality**,
two of the many factors that can affect task **turnaround** times, which consequently affect overall workflow execution performance.
With these concepts in mind, proceed to the next
 activity, [Activity 2: Parallelism]({{ site.baseurl }}/activities/activity_2), where we construct a more complex distributed computing environment in order to explore the concept of "parallelism".
