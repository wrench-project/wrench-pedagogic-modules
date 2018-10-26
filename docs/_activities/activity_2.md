---
layout: page
title: 'Activity 2: Parallelism'
---

1. [Learning objectives](#learning-objectives)
2. [Overview](#overview)
3. [Activity](#activity)
4. [Conclusion](#conclusion)

# Learning Objectives
- Gain exposure to a simple cluster computing platform;
- Understand how parallelism affects workflow execution times;
- Be able to estimate workflow execution times when tasks are executed in parallel;
- Be able to compute core utilization for multi-core platforms;
- Understand the tradeoffs between parallelism and utilization;
- Be able to compare alternative strategies for executing tasks in parallel;

# Overview
## Workflow and Platform Scenario
<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/workflow.svg">Workflow</object>

The workflow depicted in Figure 2.1 will be used in this activity. It consists of 20 computationally
intensive, independent tasks followed by a less intensive final task that depends on the previous 20 tasks. The term "independent" refers
to a set of tasks that may all be executed concurrently. The structure of this workflow appears
to be "joining" multiple tasks into one, so naturally this structure is typically called a "join"
(note that this has nothing to do with a database type of "join").   

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/cyber_infrastructure.svg">Cyber Infrastructure</object>


Figure 2.2 depicts this activity's cyber infrastructure. We build upon the
basic cyber infrastructure that was introduced in the previous activity.
The Compute Service (CS) now has a configurable number of resources on
which it can execute tasks. For example, the CS could have access to two
physical machines, or "compute nodes", both equipped with
dual-core processors and 32 GB of RAM. When the CS receives a job
containing multiple tasks, it may execute these tasks concurrently across
multiple compute nodes. The CS in this activity is what is known as a
"cluster" and can be decomposed into two parts: a "frontend node" and
"compute node(s)". The frontend node handles job requests from the Workflow
Management System (WMS) and dispatches work to the compute node(s)
according to the WMS's instructions. In this activity, our WMS submits
all workflow tasks to the CS at once, specifying for each task on which 
compute node it should run, trying to utilize the available compute nodes
as much as possible.  Connecting the CS's frontend node and compute nodes are 
high-bandwidth, low latency-network links going from each machine to a
centralized switch, which also serves as the gateway for network traffic
entering and exiting the cluster's local area network.


You will start this activity by using a CS with only a single compute node.
We will then augment the CS with more cores and more nodes to see how, and
why, the execution of our workflow is affected. Processor speed and RAM
capacity of a compute node are kept constant throughout this entire
activity.

## WMS Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/wms.svg">WMS</object>

The WMS implementation in this activity submits tasks to CS with the
following instructions regarding file operations: read the initial input
files from the remote storage service, write the final output file to the
remote storage service, read and write all other files using the CS's
scratch space. Scratch space is another name for temporary, non-volatile
storage that computing services have access to while jobs are being
executed. Having scratch space on the CS is key to enabling
data locality, which is itself key to better performance, as we learned
in the previous [Activity 1]({{ site.baseurl }}/activities/activity_1).

# Activity

## Step 1: Run the Simulation With a Single Compute Host

Here we will explore two fundamental concepts: **parallelism** and **utilization**.

**Parallelism**. Multi-core architectures enable computers to execute
multiple instructions at the same time, and in our case, multiple
independent tasks. For example, given a workflow of two independent,
identical tasks and a CS with a dual-core compute node, it is possible to
execute the workflow in the same amount of time it would take to execute
that workflow if it had only a single task.  Clearly, we can get things
done faster this way and therefore parallelism should be taken advantage
of.  More cores, however, does not mean that they cab always be used to the
fullest extent. This is because the amount of possible parallelism can be
limited by the structure of the application and/or the available compute
resources. To avoid being wasteful with our resources, it is crucial to
understand how well or badly we are utilizing them.

**Utilization**. The utilization of a core while executing a given workload is defined as follows: (compute time) / (compute time + idle time). The utilization of a multi-core platform then, is defined as the average core utilization.  For instance, consider a dual-core platform that executes a workflow for 1 hour. The first core computes for 30min, and then is idle for 30 min. The second core is idle for 15 minutes, and then computes for 45 minutes. The first core's utilization is 50%, and the second core's utilization is 75%.  The overall utilization is thus 62.5%.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/utilization.svg">Utilization</object>

Figure 2.4 illustrates the concept of utilization. The area of the container rectangle (in this example a 2-by-60 rectangle) represents the total amount of time all
cores could have computed for. The colored area represents how much time all cores actually computed for. The proportion of
the colored area to the total area tells us the utilization of this host for a given workload. Optimizing for utilization
means maximizing the colored area.   

For the remainder of this activity, we will be using the visualization tool. In the terminal, run the following
commands:
1. run `docker pull wrenchproject/wrench-pedagogic-modules:activity-visualization`
2. then run `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:activity-visualization`
3. open a browser and go to [localhost:3000/activity_2](localhost:3000/activity_2)

**Answer these questions**

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_1.svg">Compute Service 1</object>

  - [q1] Assuming the cluster has 1 single-core compute node (Figure 2.5), what do you expect the makespan of the workflow to be?
  To this end, write a simple formula. In the visualization tool,  
  set the cluster to have 1 single-core node. Run the simulation and check your answer.
  - [q2] Based on the dependencies present in the workflow, what tasks could we potentially
  execute in parallel assuming we had at least 20 cores instead of 1 core?

{% comment %}

q1. top level of workflow: 20((100MBps / 125MBps) + 3600s) = 72016s
     bottom level of workflow: (100MBps / 125MBps) + 300s = 300.8s
     total estimated time: 72317s
     actual simulated time: 72321.620633s

q2. all of the top level tasks   

{% endcomment %}

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_2.svg">Compute Service 2</object>

  - [q3] Assuming the cluster has 1 10-core compute node (Figure 2.6), what do you expect the makespan of the workflow to be?
  To this end, write a simple formula. In the visualization tool, set the cluster to have 1 10-core node.
   Run the simulation and check your answer.
  - [q4] By inspecting the Host Utilization Gantt chart in the visualization tool, is the compute node utilization greater than 50%? Explain.
  - [q5] What percentage of the compute node is being utilized throughout workflow execution?
  Write a formula for the utilization and compute the value as a percentage.

{% comment %}

q3. top level of workflow: 2(10 * (100MB / 125MBps) + 3600s) = 7216s
     bottom level of workflow: (100MB / 125MBps) + 300s = about 301s
     total estimated time: 7517s
     actual simulated time: 7521.490982

q4. There is much more yellow than pink in the Gantt chart, so the utilization has to be > 50%.

q5. (10 * (2*3600) + 9 * 0  + 1 * 300) / (10 * 7521.48783) = 96%  

{% endcomment %}

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_3.svg">Compute Service 3</object>

  - [q6] Assuming the cluster has 1 15-core compute node (Figure 2.7), what do you expect the makespan of the workflow to be?
  To this end, write a simple formula. In the visualization tool, set the cluster to have 1 15-core node.
  Run the simulation and check your answer.
  - [q7] What percentage of the compute node is being utilized while executing this workflow?
  Write a formula for the utilization and compute the value as a percentage.
  - [q8] What did we gain (or lose) by switching from a 10 core node to a 15 core node?

{% comment %}

q6. same as above
     actual simulated time: 7521.509456

q7. ((15*3600) + (5*3600) + (1*300)) / (15 * 7521.5) = 64%

q8. execution wasn't faster and utilization decreased..

{% endcomment %}

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_4.svg">Compute Service 4</object>

Assuming the cluster has 1 20-core node:
  - [q9] At what time should task T1 start?
  - [q10] At what time should task T1 end?
  - [q11] At what time should task T3 start?
  - [q12] At what time should task T3 end?
  - [q13] At what time should task T20 start?
  - [q14] How long can we expect the makespan of this workflow to be? To this end, write a simple formula.
    In the visualization tool, set the cluster to have 1 20-core node (Figure 2.8).
    Run the simulation and check your answer against the results.
  - [q15] How much faster did we execute the workflow on this platform compared to the initial platform that had only a single core?
  - [q16] Would adding one extra core to our machine further decrease the workflow makespan? Explain.
  - [q17] What percentage of the compute node is being utilized while executing this workflow?
  Write a formula for the utilization and compute the value as a percentage.


{% comment %}

q9. within the first second

q10. 20(100MBps / 125MBps) + 3600 = 3616, so at about 3616 + 1 or 2 seconds

q11. same as 1

q12. same as 2

q13. within a few seconds after the time mentioned in 2

q14. 20(100MBps / 125MBps) + 3600s + 300s + (100MBps / 125MBps) = 3917s

     actual simulated time: 3921.515231s

q15. ((72317 - 3921) / 72317) = 0.94

q16. no, the most cores we can take advantage of is 20 at the top level. then, due to dependencies, an extra core doesn't
     help us execute the bottom level any faster as well

q17. ((20*3600) + (1*300)) / (20 * 3921.5) = 0.92

{% endcomment %}

## Step 2: Augment the Compute Service to Have Multiple Compute Nodes

In Step 1, we executed the workflow under the assumption that tasks require
no RAM. Real-world workflow tasks (and programs in general) usually require
some amount of RAM to be available in the system. This step introduces a
RAM requirement for our workflow such that each task consumes 9 GB of RAM
during its execution. If a compute node does not have enough RAM to execute
the task, its execution is deferred by the CS until the required amount of RAM
becomes available on that compute node (in other terms, we do not allow swapping - see your OS
course!). Since our hosts have 32 GB of RAM, this means that at most 3
tasks can run concurrently on a host (because 4 times 9 is greater than
32). The following questions reveal how this requirement forces us to find
another means of utilizing parallelism and increasing workflow execution
performance.

**Answer these questions**

  - [q18] Assuming the cluster has 1 20-core node, and that **Workflow Tasks each require 9 GB of RAM**, what can we expect the makespan of the
  workflow to be? Write a simple formula. In the visualization tool, set the simulator to have a single compute node with 20 cores (Figure 2.8).
  Check the box that says, "Workflow Tasks use 9 GB of RAM". Run the simulation and check your results against the simulator.
  - [q19] Set the number of cores to be 64 and check the box that says, "Workflow Tasks use 9 GB of RAM". Run the simulation.
  What is the makespan of the workflow?
  - [q20] Why doesn't going to 64 cores improve workflow execution performance?
  - [q21] In fact, what is the minimum number of cores on the host that will give us the same performance?

{% comment %}

q18. 20(100MBps / 125MBps) + ceil(20 / 3)*3600s + 300s + (100MB / 125MBps) = 25517s
     actual simulated time: 25520.804330s

q19. stays the same

q20. the ram requirement makes it so that we can execute at most 3 tasks at a time on a host

q21. 3 cores

{% endcomment %}

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_5.svg">Compute Service 5</object>

  - [q22] Assuming the cluster has 5 4-core compute nodes, what can we expect the makespan of the workflow to be?
    Write a simple formula. Now set the simulator to have 5 compute nodes, each with 10 cores. Check the box that
    says "Workflow Tasks use 9 GB of RAM". Run the simulation and check your results against the simulator.
  - [q23] How much faster did the workflow execute in this execution when compared to the previous one?
  - [q24] What about the utilization of the cluster? Compute it as a percentage using a simple formula.
  - [q25] Assuming we had been able to purchase 3-core computers, what would the utilization have been?
  - [q26] Assuming that you can add an arbitrary number of 4-core nodes, with the same per-core compute speed,
  estimate the fastest possible execution time for this workflow?
  - [q27] What is the minimum number of 3-core nodes that achieves this fastest possible execution time?

{% comment %}

q23. 20(100MBps / 125MBps) + ceil(20 / (3 * 5))*3600s + 300s + (100MBps / 125MBps) = 7517s
     actual simulated time: 7521.493764s

  ((25520 - 7521) / 25520) = 0.705

q24. ((15*3600) + (5*3600) + (1*300)) / (20 * 7521.5) = 0.48

q25. ((15*3600) + (5*3600) + (1*300)) / (15 * 7521.5) = 0.64
 
q26. 20(100MBps / 125MBps) + 3600s + 300s + (100MBps / 125MBps) = 3916s
     actual simulated time: 7521.493764s

q27.  ceil(20 / 3)  = 7 nodes, then the top level can be done completely in parallel

{% endcomment %}  

When you are finished using the visualization tool, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:activity-visualization)`

# Conclusion

In this activity, you were presented with a workflow containing a set of tasks that could be executed in
parallel. To achieve better performance, we first attempted to "vertically" scale the compute service by adding
more cores to a compute node (in practice, vertically scaling can also mean purchasing a faster processor).
Your results showed that parallel execution of tasks did in fact increase workflow execution performance to a
certain degree. Next, we calculated utilization when using a different number of cores. These results should
have demonstrated to you that we can't always execute workflows as fast as possible while achieving 100% utilization.

After being introduced to parallelism and utilization, we added the
requirement of RAM usage to the workflow. in order to simulate a situation
more relevant to actual practice. Under those circumstances, the workflow
execution performance collapsed when running this workflow on a single node
CS. Rather than simply adding more cores to the single compute node, we
grew the cluster "horizontally" by adding more compute nodes. Using this strategy, we were
able to increase workflow execution performance up to a certain point.

Sometimes adding more cores or extra machines does nothing except decrease utilization.
But most importantly, both strategies, when used judiciously, can be beneficial. Therefore,
it is crucial to be cognizant of the hardware resources a workflow demands in addition to dependencies
present in its structure.   

Head over to the next section, [Activity 3: The Right Tool for the Job]({{ site.baseurl }}/activities/activity_3), where we present
you with a case study on purchasing hardware infrastructure to improve the performance of a
specific workflow execution.

{% comment %}
saving this for intro of activity 4

Then, in "Activity 4: Name TBD", we will explore more complicated workflow scenarios.
The files used in this workflow were relatively small, and the workflow tasks were more or less homogeneous. What happens
when files are large and tasks require varying amounts of computation to complete?
{% endcomment %}