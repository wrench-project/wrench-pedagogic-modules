---
layout: page
title: 'Activity 0: What is this about?'
---

The goal of these WRENCH pedagogic modules is for
students to achieve learning objectives in the area of distributed computing, in the context of scientific workflows, via hands-on experiments.  
This is achieved through a set of "*activities*" that explore, at first introductory and later sophisticated, concepts and techniques.  All of these activities entail *simulating* workflow executions, which is done with the [WRENCH](http://wrench-project.org/) simulation framework.  

Before proceeding to [Activity 1]({{ site.baseurl }}/activities/activity_1), we provide here necessary background information and terminology:

1. [What is a workflow?](#what-is-a-workflow)
2. [What kind of resources and infrastructure are necessary to execute a workflow?](#what-kind-of-resources-and-infrastructure-are-necessary-to-execute-a-workflow)
3. [What is a workflow management system?](#what-is-a-workflow-management-system)
4. [Simulating workflow executions](#simulating-a-workflow)


## What is a workflow?

**Workflows**. A workflow (a.k.a. "scientific workflow") application is comprised of individual computational tasks that together produce desired output (e.g., all the steps necessary to perform complex genomic analyses can be organized as a bioinformatics workflow). In many relevant cases, the tasks, which can be many and computational intensive, correspond to executables that read in input files and produce output files.  A file produced as output by one task can be required as input for another task, thus creating dependencies between tasks.   

**Simple Workflow Analogy**. Consider a chef tasked with cooking a meal. The entire task can be split up into three steps. First, she needs to select and procure the ingredients. Second, she needs to cook these ingredients. Finally, the cooked ingredients must be plated. None of these tasks may be completed out of order. Now consider a scientist with terabytes of raw data tasked with analyzing that data. First, she needs to do some preliminary processing of the raw data to transform it into a workable format. Second, the formatted data must go through a computationally intensive process that outputs a human readable visualization of the formatted data. Finally, she analyzes the data and distills some useful information about the origins of our universe. Again, none of these tasks may be done out of order. Although the two workflows mentioned above come from entirely different domains, they share several characteristics. First, the initial task may be started immediately because it has no dependencies (some workflows may have multiple initial tasks without any dependencies). Second, subsequent tasks can only be started once their "parent" tasks have completed. This leads to a simple representation of workflows as tasks graphs.

**Workflows as DAGs**. Workflows can be represented as graphs in which tasks are vertices and task dependencies are edges. In many cases, these workflow graphs are Directed Acyclic Graphs (DAGs) because there are no circular data dependencies.  Once formalized as DAGS, it is possible to reason about workflow structure to try to organize workflow execution as best as possible.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/dag.svg">Dag</object>

Some examples of real-world workflows for scientific applications, along with their DAG representations, can be found [here](https://pegasus.isi.edu/application-showcase/).

## What kind of resources and infrastructure are necessary to execute a workflow?

Workflows are often comprised of many tasks that are computationally intensive and require large amounts of storage. As a result, it is necessary to deploy workflow executions on multiple compute/storage resources connected via some network, i.e., a distributed computing platform. These hardware resources are managed by software infrastructure, together forming a "*cyber infrastructure*" (a term you may have encountered before). Examples of such infrastructures include cloud services that rely on virtual machines,  batch-scheduled high performance clusters (a.k.a. "supercomputers"), publicly available data stores that provide data access using various network protocols, etc. Communications between these resources are subject to latency and bandwidth constraints, therefore the underlying network infrastructure in conjunction with the specifications of the actual resources both contribute to the overall execution of applications, and in our case workflow applications. Due to the complexity and diversity of these infrastructures, users are not left to their own devices for executing their workflows. Instead, they rely on software tools that orchestrate workflow executions automatically. These tools are known as *Workflow Management Systems*.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/infrastructure.svg">Infrastructure</object>

## What is a workflow management system?

A workflow management system (WMS) is a program that executes workflow tasks on available resources, moves data where it is needed, and handles failures. Furthermore, most WMSs implement decision-making algorithms for optimizing workflow execution performance. WMS and the algorithms they employ are the object of extensive research in the field of distributed computing.  

## Simulating a workflow execution

Evaluating the performance of a WMS with a given workflow on an arbitrary set of resources can be costly and time consuming. Furthermore, accurately reproducing such test results presents itself as a challenging endeavor as real-world distributed platforms often have dynamic and unstable behaviors. Furthermore, such evaluations would be limited to resources at hand, precluding the exploration of "what if?" scenarios. These limitations can be addressed by using *simulation* so as to quantify the performance of a particular WMS implementation in a way that is repeatable while allowing the exploration of arbitrary experimental scenarios. Both the underlying infrastructure and compute resources can be accurately simulated using [WRENCH](http://wrench-project.org/), a WMS simulation framework. A benefit of this approach is that the simulation will run orders of magnitude faster than the actual time it would take to execute the workflow in real life. Additionally, the simulation is scalable and can be executed on, say, a single laptop computer. [WRENCH](http://wrench-project.org/) thus provides a simulation foundation for aiding workflow and WMS research, development, and education.  In these pedagogic modules we use [WRENCH](http://wrench-project.org/) for educational purposes.

Continue to the next section,
[Activity 1: Running Your First Simulated Workflow Execution]({{ site.baseurl }}/activities/activity_1), where we will begin running basic simulations.
