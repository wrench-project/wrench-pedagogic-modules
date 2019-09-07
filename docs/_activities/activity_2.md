---
layout: page
title: 'Activity 2: Parallelism'
order: 40
usemathjax: true
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
- Be able to compare alternative strategies for executing tasks in parallel.

# Overview
## Workflow and Platform Scenario
<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/workflow.svg">Workflow</object>

In this activity we use the workflow depicted in Figure 1. It consists of 20 computationally
intensive, independent tasks followed by a less intensive final task that depends on the previous 20 tasks.
The term "independent" refers to a set of tasks that may all be executed concurrently. Additionally, each task now has a
RAM requirement such that the amount of RAM a task uses is equal the sum of its input and output file sizes.

The structure of this workflow appears
to be "joining" multiple tasks into one, so naturally this structure is typically called a "join"
(note that this has nothing to do with a database type of "join").   

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/cyber_infrastructure.svg">Cyberinfrastructure</object>


Figure 2 depicts this activity's cyberinfrastructure. We build upon the
basic cyberinfrastructure that was introduced in the previous activity.
The Compute Service (CS) now has a configurable number of resources on
which it can execute tasks. It has access to several
physical machines, or "compute nodes", equipped with
dual-core processors and 80 GB of RAM. When the CS receives a job
containing multiple tasks, it may execute these tasks concurrently across
multiple compute nodes. The CS in this activity is what is known as a
"cluster" and can be decomposed into two parts: a "frontend node" and
"compute node(s)". The frontend node handles job requests from the Workflow
Management System (WMS) and dispatches work to the compute node(s)
according to some decision-making algorithm.
In this activity, our WMS submits
all workflow tasks to the CS at once, specifying for each task on which
compute node it should run, trying to utilize the available compute nodes
as much as possible.  

Connecting the CS's frontend node and compute nodes are
high-bandwidth, low latency-network links going from each machine to a
centralized switch, which also serves as the gateway for network traffic
entering and exiting the cluster's local area network. This means that
a file being transferred from the Remote Storage Service at
*storage_db.edu* to the CS at *hpc.edu* must travel through two links:
first the link between *storage_db.edu* and the switch, then the link
between the switch and the frontend node at *hpc.edu/node_0*. Say that
the file is 3000 MB, based on what we learned from the
[primer on file transfer times]({{ site.baseurl }}/activities/primer_on_file_transfer_times),
we expect the duration of this file transfer to be as follows:

$$
\begin{align}

T_{3000\;MB\;file} & = \sum_{l \in route} Latency(l) + \frac{m}{\min\limits_{l \in route} Bandwidth(l)} \\
                   & = (100us + 10us) + \frac{3000\;MB}{125\;MB/sec} \\
                   & = 0.000110\;sec + 24\;sec \\
                   & = 24.000110\;seconds\\
                   & \simeq 24\;seconds
\end{align}
$$

Furthermore, when a compute node reads a file from scratch space, the file
will travel through the two links separating the compute node and the frontend
node. For example, if the compute node at *hpc.edu/node_1* reads a 3000 MB
file from the CS's scratch space at *hpc.edu/node_0*, the expected duration
for that read operation is as follows:

$$
\begin{align}

T_{3000\;MB\;file} & = \sum_{l \in route} Latency(l) + \frac{m}{\min\limits_{l \in route} Bandwidth(l)} \\
                   & = (10us + 10us) + \frac{3000\;MB}{1250\;MB/sec} \\
                   & = 0.000020\;sec + 2.4\;sec \\
                   & = 2.40002\;seconds\\
                   & \simeq 2.4\;seconds

\end{align}
$$

You will start this activity by using a CS with only a single compute node.
You will then augment the CS with more cores and more nodes to see how
the execution of our workflow is affected. Individual processor speed and RAM
capacity of compute nodes are kept constant throughout this entire
activity.

## WMS Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/wms.svg">WMS</object>

The WMS implementation in this activity submits tasks to the CS using the
following scheme regarding file operations: read the initial input
files from the remote storage service, write the final output file to the
remote storage service, read and write all other files using the CS's
scratch space. Scratch space is another name for temporary, non-volatile
storage that computing services have access to while jobs are being
executed. Having a scratch space on the CS is key to enabling
data locality, which is itself key to better performance, as we learned
in the previous [Activity 1]({{ site.baseurl }}/activities/activity_1). This
scheme is depicted in Figure 3.

# Activity

## Step 1: Run the Simulation With a Single Compute Host

Now we explore two fundamental concepts: **parallelism** and **utilization**.

**Parallelism**. Multi-core architectures enable computers to execute
multiple instructions at the same time, and in our case, multiple
independent tasks. For example, given a workflow of two independent,
identical tasks and a CS with a dual-core compute node, it may be possible to
execute the workflow in the same amount of time it would take to execute
that workflow if it had only a single task.  One can take advantage of
such *parallelism* to get things
done faster.
More cores, however, does not mean that they can always be used to the
fullest extent. This is because the amount of possible parallelism can be
limited by the structure of the application and/or the available compute
resources. To avoid being wasteful with our resources, it is crucial to
understand how well or badly we are utilizing them.

**Utilization**. The utilization of a core while executing a given workload is
defined as follows: (compute time) / (compute time + idle time). The
utilization of a multi-core host then, is defined as the average core
utilization.  For instance, consider a dual-core host that executes a workflow
for 1 hour. The first core computes for 30 min, and then is idle for 30 min.
The second core is idle for 15 minutes, and then computes for 45 minutes.

$$
\begin{align}

average\;core\;utilization_{host} & = \frac{1}{\left| cores \right|} \sum_{c \in cores}\frac{compute\;time(c)}{compute\;time(c) + idle\;time(c)} \\
                                  & = \frac{1}{2} \left( \frac{30\;min}{30\;min + 30\;min} + \frac{45\;min}{45\;min + 15\;min}  \right) \\
                                  & = \frac{1}{2} \left( \frac{1}{2} + \frac{3}{4} \right) \\
                                  & = 0.625

\end{align}
$$

The first core's utilization is 50%, and the second core's utilization is 75%.
The overall utilization is thus 62.5%.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/utilization.svg">Utilization</object>

Figure 4 illustrates the concept of utilization. The area of the container
rectangle (in this example a 2-by-60 rectangle) represents the total amount of
time all cores could have computed for. The colored area represents how much
time all cores actually computed for. The proportion of the colored area to the
total area tells us the utilization of this host for a given workload.
Optimizing for utilization means maximizing the colored area.   

For the remainder of this activity, we will be using the visualization tool. In the terminal, run the following
commands:
1. run `docker pull wrenchproject/wrench-pedagogic-modules:activity-visualization`
2. then run `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:activity-visualization`
3. open a browser and go to [localhost:3000/](localhost:3000/)
4. sign in using your `<UH Username>@hawaii.edu` Google Account
5. select `Activity 2: Parallelism`

**Answer these questions**

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_1.svg">Compute Service 1</object>

  - [q1] Assuming the cluster has 1 single-core compute node (Figure 5), what do you expect the overall execution time, or *makespan*, of the workflow to be?
  To this end, write a simple formula. In the visualization tool,  set the cluster to have 1 single-core node. Run the simulation and check your answer. (Note that you might not be able to see file transfer operations in the displayed Gantt charts because these operations could be very short relatively to the overall makespan.)
  - [q2] Based on the dependencies present in the workflow, what tasks could we potentially execute in parallel assuming we had at least 20 cores instead of 1 core?

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_2.svg">Compute Service 2</object>

  - [q3] Assuming the cluster has 1 10-core compute node (Figure 6), what do you expect the makespan of the workflow to be?
  To this end, write a simple formula. In the visualization tool, set the cluster to have 1 10-core node.
   Run the simulation and check your answer.
  - [q4] By inspecting the Host Utilization Gantt chart in the visualization tool, is the compute node utilization greater than 50%? Explain.
  - [q5] What percentage of the compute node is being utilized throughout workflow execution?
  Write a formula for the utilization and compute the value as a percentage.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_3.svg">Compute Service 3</object>

  - [q6] Assuming the cluster has 1 15-core compute node (Figure 7), what do you expect the makespan of the workflow to be?
  To this end, write a simple formula. In the visualization tool, set the cluster to have 1 15-core node.
  Run the simulation and check your answer.
  - [q7] What percentage of the compute node is being utilized while executing this workflow?
  Write a formula for the utilization and compute the value as a percentage.
  - [q8] What did we gain (or lose) by switching from a 10 core node to a 15 core node?

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_4.svg">Compute Service 4</object>

Assuming the cluster has 1 20-core node:
  - [q9] At what time would task T0 start?
  - [q10] At what time would task T19 start?
  - [q11] At what time would task T20 start?
  - [q12] How long can we expect the makespan of this workflow to be? To this end, write a simple formula.
    In the visualization tool, set the cluster to have 1 20-core node (Figure 8).
    Run the simulation and check your answer against the results.
  - [q13] How much faster did we execute the workflow on this platform compared to the initial platform that had only a single core?
  - [q14] Would adding one extra core to our machine further decrease the workflow makespan? Explain.
  - [q15] What percentage of the compute node is being utilized while executing this workflow?
  Write a formula for the utilization and compute the value as a percentage.


## Step 2: Augment the Compute Service to Have Multiple Compute Nodes

In Step 1, each workflow task had a RAM requirement such that its RAM
usage was equal to the sum of its input and output file sizes. What about
RAM required for the task itself? That is, real-world workflow tasks (and
programs in general) usually require some amount of RAM for storing program instructions,
possibly large temporary data-structures, the runtime stack, etc. In this step we thus introduce an *additional*
12 GB RAM requirement for each task (Figure 9). For example, *task0* previously required
4 GB of RAM, whereas in this step it requires 16 GB of RAM.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/workflow_using_additional_ram.svg">Workflow With Additional RAM Requirement</object>

If a compute node does not have enough RAM to execute
the task, its execution is deferred by the CS until the required amount of RAM
becomes available on that compute node (in other terms, we do not allow swapping - see your OS
course!). Since our hosts have 80 GB of RAM, this means that at most 5
tasks can run concurrently on a host (because 6 times 16 is greater than
80). The following questions reveal how this requirement forces us to find
another means of utilizing parallelism and increasing workflow execution
performance.

**Answer these questions**

  - [q16] Assume that the cluster has 1 20-core node and that **Workflow Tasks each require an additional 12 GB of RAM, meaning that they require 16 GB**.
  What can we expect the makespan of the workflow to be? Write a simple formula. In the visualization tool,
  set the simulator to have a single compute node with 20 cores (Figure 8).
  Check the box that says, "Workflow Tasks each require an additional 12 GB of RAM". Run the simulation and check your results against the simulator.
  - [q17] Set the number of cores to be 32 and check the box that says, "Workflow Tasks each require an additional 12 GB of RAM". Run the simulation.
  What is the makespan of the workflow?
  - [q18] Why doesn't going to 32 cores improve workflow execution performance?
  - [q19] In fact, what is the minimum number of cores on the host that will give us the same performance?

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_2/compute_service_5.svg">Compute Service 5</object>

  - [q20] Assuming the cluster has 4 8-core compute nodes (Figure 10), what can we expect the makespan of the workflow to be?
    Write a simple formula. Now set the simulator to have 4 compute nodes, each with 8 cores. Check the box that
    says "Workflow Tasks each require an additional 12 GB of RAM". Run the simulation and check your results against the simulator.
  - [q21] How much faster did the workflow execute in this execution when compared to the previous one (the answer from q16)?
  - [q22] What about the utilization of the cluster? Compute it as a percentage using a simple formula.
  - [q23] Assuming we had been able to purchase 4 5-core compute nodes instead of 4 8-core compute nodes, what would the utilization have been?
  - [q24] Assuming that you can add an arbitrary number of 5-core nodes, with the same per-core compute speed, is it possible to
          decrease the workflow execution time?
  - [q25] What is the minimum number of 3-core nodes that achieves this fastest possible execution time?

When you are finished using the visualization tool, run: `docker kill $(docker ps -a -q  --filter ancestor=wrenchproject/wrench-pedagogic-modules:activity-visualization)`

# Conclusion

In this activity, you were presented with a workflow containing a set of tasks that could be executed in
parallel. To achieve better performance, you first attempted to "vertically" scale the compute service by adding
more cores to a compute node (in practice, vertically scaling can also mean purchasing a faster processor).
Your results showed that parallel execution of tasks did in fact increase workflow execution performance to a
certain degree. Next, you calculated utilization when using a different number of cores. These results should
have demonstrated to you that one cannot always execute workflows as fast as possible while achieving 100% utilization.

After introducing parallelism and utilization, you added
additional RAM requirements to the workflow in order to simulate a situation
more relevant to actual practice. Under those circumstances, the workflow
execution performance collapsed when running this workflow on a single node
CS. Rather than simply adding more cores to the single compute node, you
grew the cluster "horizontally" by adding more compute nodes. Using this strategy, you were
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
