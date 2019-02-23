---
layout: page
title: 'What Is This About?'
order: 0
usemathjax: true
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

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/what_is_this_about/dag.svg">Dag</object>

Some examples of real-world workflows for scientific applications, along with their DAG representations, can be found [here](https://pegasus.isi.edu/application-showcase/).

## What kind of resources and infrastructures are necessary to execute a workflow?

Workflows are often comprised of many tasks that are computationally intensive and require large amounts of storage. As a result, it is necessary to deploy their executions on multiple compute/storage resources connected via some network, i.e., distributed computing platforms. These hardware resources are managed by software infrastructures, together forming a "*cyber infrastructure*" (a term you may have encountered before). Examples of such infrastructures include cloud services that rely on virtual machines,  batch-scheduled high performance clusters (a.k.a. "supercomputers"), publicly available data stores that provide data access using various network protocols, compositions of any number of theses over possibly wide-area networks, etc. Communications between these resources are subject to network latency and bandwidth constraints. Therefore the underlying network infrastructure, in conjunction with the specifications of the interconnected storage and compute resources,  constrains the performance of distributed applications, in our case workflow applications. Due to the complexity and diversity of these infrastructures, users are not left to their own devices for executing workflows. Instead, they rely on software tools that orchestrate workflow executions automatically. These tools are known as *Workflow Management Systems*.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/what_is_this_about/infrastructure.svg">Infrastructure</object>

## What is a Workflow Management System?

A Workflow Management System (WMS) is a program that automatically orchestrates the execution of workflows on cyber infrastructures. To do so, most WMSs implement decision-making algorithms for optimizing workflow execution performance given available hardware resources. WMS and the algorithms they employ are the object of extensive research in the field of distributed computing.

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

When going through these pedagogic modules, you will need to do back-of-the-envelope calculations
of durations of computation and of file transfer times.  While estimating computation times
is straightforward, estimating data transfer times is a bit more involved and may be something
to which you have not been exposed previously.  To make sure you are well-prepared for the upcoming
pedagogic activities, we provide you with a [a brief primer on file transfer times]({{ site.baseurl }}/activities/primer_on_file_transfer_times)
