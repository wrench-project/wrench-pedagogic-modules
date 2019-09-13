---
layout: page
title: 'F. Workflows and Data Locality'
order: 600
---

1. [Learning objectives](#learning-objectives)
2. [Overview](#overview)
3. [Activity](#activity)
4. [Conclusion](#conclusion)


#### Learning Objectives

  - Understand I/O overhead effects on workflow executions;
  - Gain exposure to the concept of data locality and its effect on workflow execution.


#### Workflow and Platform Scenario

In this activity, we study the execution of the workflow depicted in Figure 1 below. 

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/workflow.svg">Workflow</object>

We wish to execute this workflow on the cyberinfrastructure depicted in
Figure 2 below (which is the same as that used in the previous module). To
summarize, a Compute Service (CS) at host `hpc.edu` has a single core that
can execute workflow tasks (but only one at a time). Workflow files are
stored on the Storage Service (SS) at host `storage_db.edu` that handles
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

This will take you to a Web app.  For now, just click on the "Run
Simulation" button, without changing the content of the text box or clicking
any radio button. As in the previous module, the Web app displays textual and
visual simulation output.  

**Based on this output, answer the following questions:**

  - [q1] Is the workflow execution I/O-intensive or compute-intensive?
  - [q2] If the link bandwidth between `storage_db.edu` and `hpc.edu` were
         doubled, what fraction of *task4*'s execution time would be spent doing I/O?
  - [q3] Double the platform link bandwidth (set it to 20 MB/sec) using the visualization and run the simulation. Is your expectation in q2 confirmed?
  - [q4] Using analysis (i.e., a simple equation), determine the link bandwidth that would be necessary for the workflow to run 2x faster than with the original 10 MB/sec bandwidth? 
  - [q5] Using the simulator report on the accuracy of the result from your analysis in q4.


#### Data Locality

In the workflow execution above, the CS reads and writes files from and to
the SS on `storage_db.edu`, which causes a lot of I/O overhead. This
overhead can be vastly reduced if the storage service were located on the
same host as the CS (which you can think of as a disk on which data can be
cached at the SS). The idea would thus be to data *closer* to where the
computation is taking place, or improving **data locality**.

Figure 3 below shows on the left-hand side a depiction of the new
cyberinfrastructure, on which a second SS is started at host `hpc.edu`.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/scenario_2.svg">Scenario 2</object>

We have enhanced the WMS implementation so that it  can take advantage of
the new SS for storing all the "intermediate" workflow files, i.e., those
files shown in red on the right-hand side of Figure 3 above. The only files
that need to be transferred from/to the SS on `storage_db.edu` will thus be
initial *task0::0.in* file and the final output file, *task4::0.out*.

The above enhancement can be activated by selecting the radio button that
says *Storage Service on storage_db.edu and hpc.edu*. Leaving the bandwidth
at 10 MB/sec, select the radio button and click the "Run Simulation"
button.

**Answer the following questions based on the simulation output**:

  - [q6] What fraction of *task4* is spent doing I/O?

  - [q7] Compared to the workflow execution time when there is a single SS, how much faster is it?

  - [q8] Assuming a single SS (the one on `storage_db.edu`), what would you need to increase the bandwidth of the link between `storage_db.edu` and `hpc.edu` in order to have a workflow execution that is faster than what was realized using a 10 MB/sec link bandwidth when there are two SS. 

When you are finished using the Web app, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:ics332-activity-visualization)`

---
