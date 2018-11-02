---
layout: page
title: 'Activity 0: What Is This About?'
---

The goal of these WRENCH pedagogic modules is for
students to achieve learning objectives in the area of distributed computing, in the context of scientific workflows, via hands-on experiments.  
This is achieved through a set of "*activities*" that explore, at first introductory and later sophisticated, concepts and techniques.  All of these activities entail *simulating* workflow executions, which is done with the [WRENCH](http://wrench-project.org/) simulation framework.  

Before proceeding to [Activity 1]({{ site.baseurl }}/activities/activity_1), we provide here necessary background information and terminology:

1. [What is a workflow?](#what-is-a-workflow)
2. [What kind of resources and infrastructures are necessary to execute a workflow?](#what-kind-of-resources-and-infrastructures-are-necessary-to-execute-a-workflow)
3. [What is a workflow management system?](#what-is-a-workflow-management-system)
4. [Simulating workflow executions](#simulating-a-workflow-execution)
5. [Conclusion](#conclusion)

## What is a workflow?

**Workflows**. A workflow (a.k.a. "scientific workflow") application is comprised of individual computational tasks that must all be execute in some particular sequence to produce a final desired output (e.g., all the steps necessary to perform complex genomic analyses can be organized as a bioinformatics workflow). In many relevant cases, the tasks, which can be many and computationally intensive, correspond to executables that read in input files and produce output files.  A file produced as output by one task can be required as input for another task, thus creating dependencies between tasks.

**Simple Workflow Analogy**. Consider a chef tasked with cooking a meal. The entire task can be split up into three steps. First, they need to select and procure the ingredients. Second, they need to cook these ingredients. Finally, the cooked ingredients must be plated. None of these tasks may be completed out of order. Now consider a scientist with terabytes of raw data tasked with analyzing that data. First, they need to do some preliminary processing of the raw data to transform it into a workable format. Second, the formatted data must go through a computationally intensive process that outputs a human readable visualization of the formatted data. Finally, the scientist analyzes the data and distills some useful information about the origins of our universe. Again, none of these tasks may be done out of order. Although the two workflows mentioned above come from entirely different domains, they share several characteristics. First, the initial task may be started immediately because it has no dependencies (some workflows may have multiple initial tasks without any dependencies). Second, subsequent tasks can only be started once their "parent" tasks have completed. This leads to a simple representation of workflows as tasks graphs.

**Workflows as DAGs**. Workflows can be represented as graphs in which tasks are vertices and task dependencies are edges. In many cases, these workflow graphs are Directed Acyclic Graphs (DAGs) because there are no circular task dependencies.
Once formalized as DAGs, it is possible to reason about workflow structure so as to organize workflow execution as best as possible.

The figure below depicts example workflows. Each task is shown as a circle. In these pedagogic modules we assume that we know
the computational cost of each task (in number of floating point operations, or FLOPS). Workflow files, which are taken as input and
produced as output of the tasks, are shown as rectangle. In these pedagogic modules we assume that we know all file
sizes (in bytes).

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_0/dag.svg">Dag</object>

Some examples of real-world workflows for scientific applications, along with their DAG representations, can be found [here](https://pegasus.isi.edu/application-showcase/).

## What kind of resources and infrastructures are necessary to execute a workflow?

Workflows are often comprised of many tasks that are computationally intensive and require large amounts of storage. As a result, it is necessary to deploy their executions on multiple compute/storage resources connected via some network, i.e., distributed computing platforms. These hardware resources are managed by software infrastructures, together forming a "*cyber infrastructure*" (a term you may have encountered before). Examples of such infrastructures include cloud services that rely on virtual machines,  batch-scheduled high performance clusters (a.k.a. "supercomputers"), publicly available data stores that provide data access using various network protocols, compositions of any number of theses over possibly wide-area networks, etc. Communications between these resources are subject to network latency and bandwidth constraints. Therefore the underlying network infrastructure, in conjunction with the specifications of the interconnected storage and compute resources,  constrains the performance of distributed applications, in our case workflow applications. Due to the complexity and diversity of these infrastructures, users are not left to their own devices for executing workflows. Instead, they rely on software tools that orchestrate workflow executions automatically. These tools are known as *Workflow Management Systems*.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_0/infrastructure.svg">Infrastructure</object>

## What is a Workflow Management System?

A Workflow Management System (WMS) is a program that automatically orchestrates the execution of workflows on cyberinfrastructures. To do so, most WMSs implement decision-making algorithms for optimizing workflow execution performance given available hardware resources. WMS and the algorithms they employ are the object of extensive research in the field of distributed computing.

## Simulating a workflow execution

Evaluating the performance of a WMS with a given workflow on an arbitrary set of resources can be costly and time consuming.
Furthermore, accurately reproducing experiments that measure workflow execution performance is also a challenge.
 This is because real-world distributed platforms often have dynamic and unstable behaviors.
Finally, experiments are limited to the resources at hand, precluding the exploration of "what if?" scenarios.
These limitations make it difficult to study workflow and WMS performance experimentally.

One solution is to use *simulation* so as to quantify the performance of a particular WMS implementation in a way that is repeatable
while allowing the exploration of arbitrary experimental scenarios. This is the goal of the [WRENCH](http://wrench-project.org/) project.
With WRENCH, the simulation can run orders of magnitude faster than the actual time it would take to execute the workflow in real life.
Additionally, the simulation is scalable and can be executed on, say, a single laptop computer. [WRENCH](http://wrench-project.org/) thus provides a simulation
foundation for aiding workflow and WMS research, development, and education.
In these pedagogic modules we use [WRENCH](http://wrench-project.org/) for educational purposes.


### A brief primer on modeling networks

[WRENCH](http://wrench-project.org/) is based on the [SimGrid](http://simgrid.org) simulation framework. [SimGrid](http://simgrid.org) strives to implement realistic simulation models that capture many of the effects seen in real-world platforms.
For instance, sending 100MiB of data over a network link with a bandwidth of 1000MiB/sec and a latency of 10ms does not take exactly 1.01 seconds
on real-world networks (e.g., due to the TCP protocol, due to various network overheads), which is captured in [SimGrid](http://simgrid.org) simulations.  When going through these pedagogic modules and
inspecting execution timelines, you will thus likely note that back-of-the-envelope calculations of durations, e.g., data transfer times, do not always align with the simulation.


To demonstrate this discrepancy between back-of-the-envelope calculations and simulation derived data, we present
to you three simple scenarios where files need to be sent from one host to another.

### Scenario 1: Single file transfer

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_0/scenario_1.svg">Scenario 1</object>

*About how long should it take to send a single 100 MB file from "host1" to "host2"?* Let
$l$ be a link on the route $r$ over which the file
is being sent, and $m$ be the size of our file. Then we can estimate, naively, the file transfer time with the following:

$$
\begin{align}

 T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{\min\limits_{l \in r} Bandwidth(l)} \\
  & = 3(0.0001s) + \frac{100MB}{100MBps} \\
  & = 1.0003s

\end{align}
$$

Using [WRENCH](http://wrench-project.org/) to simulate this scenario, we realize a file transfer time of 1.098119 seconds.
As previously mentioned, [SimGrid](http://simgrid.org) models the TCP protocol and therefore the maximum observed
bandwidth will be at most 92% of the actual bandwidth of the link. We can account for this by modifying
our equation to be:

$$
\begin{align}

T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{0.92 (\min\limits_{l \in r} Bandwidth(l))} \\
  & = 3(0.0001s) + \frac{100MB}{0.92(100MBps)} \\
  & = 1.0872s

\end{align}
$$

This calculation is much closer to what was derived from the simulation, however a discrepancy still
exists between our calculation and the simulation. [SimGrid's](http://simgrid.org) rigorous approach
to modeling networks can be difficult to mimic with back-of-the-envelope calculations, especially
in situations where the state of the network is in constant flux due to changes in network traffic. As such,
you can use either of the two equations for rough estimates of file transfer times.

### Scenario 2: Two concurrent file transfers

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_0/scenario_2.svg">Scenario 2</object>

*About how long should it take to send two 75 MB files concurrently from "host1" to "host2"?* In this situation,
the bandwidth will be shared amongst the two concurrent file transfers. Let $n$ be the number of files to send
concurrently. Building off of the previous equation, we have the following:

$$
\begin{align}

T_{2files} & = \sum_{l \in r} Latency(l) + \frac{nm}{0.92 (\min\limits_{l \in r} Bandwidth(l))} \\
  & = 3(0.0001s) + \frac{2 * 75MB}{0.92(100MBps)} \\
  & = 1.63073s

\end{align}
$$

Based on the simulation results, 2 concurrent file transfers should take about 1.6335 seconds. Again, the estimate is close,
but does not align perfectly with the simulation.

### Scenario 3: Bottleneck links

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_0/scenario_3.svg">Scenario 3</object>

*About how long should it take to send a single 100 MB file from "host1" to "host2" given that the middle network link now
has a bandwidth of 10 MBps?* It is almost always the case that data will be transmitted over a heterogeneous set of
network links, hence the use of the $min$ operation. In this scenario, the throughput
of our network links are bounded at 10 MBps because of the middle bottleneck link.

$$
\begin{align}

T_{2files} & = \sum_{l \in r} Latency(l) + \frac{m}{0.92 (\min\limits_{l \in r} Bandwidth(l))} \\
  & = 3(0.0001s) + \frac{100MB}{0.92(10MBps)} \\
  & = 10.8698

\end{align}
$$

Simulation results for this scenario show that the file transfer would take 10.8406 seconds to complete.

### Try the file transfer simulation

We provide the simulation used in this activity. In a terminal, run the following commands.

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-0`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 num_files file_size center_link_bandwidth`
    - `num_files`: the number of files in the range [1, 100]
    - `file_size`: the size of each file (in MB) in the range [1, 1000]
    - `center_link_bandwidth`: the bandwidth of the center link (in MBps) in the range [1, 1000]

For example, the command `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 2 75 100` will simulate
scenario 2 and print the following output to your screen:

```
         Simulation Results
         ------------------
  files transferred: 2
          file size: 75 MB
       min duration: 1.631561678
       max duration: 1.635486847
      mean duration: 1.633524263
 standard deviation: 0.001962584536
```

Notice that there are some statistics provided about the file transfers. When transferring more than one file
concurrently, the duration it takes to transfer each file may differ slightly. Additionally, these file
transfers do not start at the exact same moment, nor do they end at the exact same moment. These are minute details 
captured by the simulation, which we cannot fully model with the equations above.  

## Conclusion

To this end, we have introduced the goal of these pedagogic modules and laid the foundation for the coming
activities. Throughout the course of each activity, you will be asked to estimate file transfer times and task
execution times given a specific set of hardware constraints. Note that your answers will not be exact, and so
we will be using simulations to verify calculations. Continue to the next section,
[Activity 1: Running Your First Simulated Workflow Execution]({{ site.baseurl }}/activities/activity_1),
where we will begin running basic [WRENCH](http://wrench-project.org/) simulations.
