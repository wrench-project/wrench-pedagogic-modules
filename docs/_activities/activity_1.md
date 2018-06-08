---
layout: page
title: 'Activity 1: Running Your First Simulated Workflow Execution'
---

## Index
1. [What is a workflow?](#what-is-a-workflow)
2. [What kind of resources and infrastructure are necessary to execute a workflow?](#what-kind-of-resources-and-infrastructure-are-necessary-to-execute-a-workflow)
3. [What is a workflow management system?](#what-is-a-workflow-management-system)
4. [Simulating a workflow execution](#simulating-a-workflow)
5. [Running the simulation](#running-the-simulation)
6. [Interpreting the results](#interpret-the-results)

## What is a workflow?

**Workflows**. A workflow (a.k.a. "scientific workflow") application is comprised of individual computational tasks that together produce desired output (e.g., all the steps necessary to perform complex genomic analyses are organized in a bioinformatics workflow). In general, the tasks, which are typically many and computational intensive, read in input files and produce output files.  A file produced as output by one task can be required as input for another task, thus creating dependencies between tasks.   

**Simple Workflow Analogy**. Consider a chef tasked with cooking a meal. The entire task can be split up into three steps. First, she needs to prepare the ingredients. Second, she needs to cook those ingredients that were just prepared. Finally, the cooked ingredients must be plated. None of those individual tasks may be completed out of order. Now consider a scientist with terabytes of raw data tasked with analyzing that data. First, she needs to do some preliminary processing of the raw data to transform it into a format that can be worked with. Second, the formatted data must go through a computationally intensive process that outputs a human readable visualization of the formatted data. Finally, she analyzes the data and distills some useful information about the origins of our universe. Again, none of these tasks may be done out of order. Although the two workflows mentioned above come from entirely different domains, they both share several common properties. First, the initial task may be started immediately because it has no dependencies. Some workflows may have multiple starting tasks without any dependencies. Second, subsequent tasks can only be started once their "parent" tasks have been completed. As we will see in the following section, directed acyclic graphs are natural representations of workflows. 

**Workflows as DAGs**. Workflows are often represented as directed acyclic graphs (DAGs) because DAGs naturally capture task dependencies. Furthermore, a DAG can be analyzed to better understand the structure of the workflow it represents. For instance, using simple graph theoretic computations one can determine which tasks can be executed concurrently (to be explored in *Activity 2: Parallelism*). 


<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/dag.svg">Dag</object>

Some examples of real world workflows and their applications can be found [here](https://pegasus.isi.edu/application-showcase/).

## What kind of resources and infrastructure are necessary to execute a workflow?

Workflows are often comprised of many tasks, which are computationally intensive and require large amounts of storage. As a result, it is necessary to deploy workflow executions on multiple compute/storage resources connected via some network so as to form a distributed computing platform. Hardware resources are managed by software infrastructure, forming a "cyberinfrastructure" (a term you may have encountered before). Examples of such infrastructures include cloud services that rely on virtual machines,  batch-scheduled high performance large clusters (a.k.a. "supercomputers"), publicly available data stores that provide data access using various network protocols, etc. Communications between these resources are subject to latency and bandwidth constraints, therefore the underlying network infrastructure in conjunction with the specifications of the actual resources both contribute to the overall execution of applications, and in our case workflow applications. Due to the complexity and diversity of these infrastructures, users are not left to their own devices for executing their workflows. Instead, they rely on software tools that orchestrate workflow executions automatically. These tools are known as *workflow management systems*. 

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/infrastructure.svg">Infrastructure</object>

## What is a workflow management system?

A workflow management system (WMS) is a program that schedules the execution of workflow tasks on available resources, moves data where it is needed, and handles failures. WMSs implement various decision-making algorithms for optimizing workflow execution performance, and are the object of extensive research in the field of distributed computing.  

## Simulating a workflow 

Evaluating the performance of a WMS with a given workflow on an arbitrary set of resources can be costly and time consuming. Furthermore, accurately reproducing such test results presents itself as a challenging endeavor as real-world distributed platforms often have dynamic and unstable behaviors. Furthermore, such evaluations would be limited to resources at hand, precluding the exploration of "what if?" scenarios. These limitations can be addressed by using simulation so as to quantify the performance of a particular WMS implementation in a way that is repeatable while allowing the exploration of arbitrary experimental scenarios. Both the underlying infrastructure and compute resources can be accurately simulated using [WRENCH](http://wrench-project.org/), a WMS simulation framework. A benefit of this approach is that the simulation will run orders of magnitude faster than the actual time it would take to execute the workflow in real life. Additionally, the simulation can be executed scalably on, say, a single laptop computer. [WRENCH](http://wrench-project.org/) thus provides a simulation foundation for aiding workflow and WMS research, development, and education. 

Three components are required to simulate a workflow execution with [WRENCH](http://wrench-project.org/): *a workflow*, *an implementation of a WMS*, and *a description of the hardware/software infrastructure the WMS can use*.

**Providing a workflow.** Workflows can be described using [DAX](https://confluence.pegasus.isi.edu/display/pegasus/WorkflowGenerator) (a representation of a DAG in xml) or [JSON](https://workflowhub.isi.edu/traces/). The following image illustrates the `workflow.dax` file *provided in this activity*.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/workflow.svg">Workflow</object>

**Implementing a WMS.** Simulating a WMS requires that the WMS be implemented in C++ using the [WRENCH](http://wrench-project.org/) "developer" API. *This activity provides a ready-to-use implementation of a simple WMS*.

**Describing the platform**. Platforms are described using XML and these descriptions contain information about the hardware resources available and the underlying network topology that connects them together. Compute resources are denoted as "hosts" and have attributes such as the number of processor cores it has and their processing power measured in floating point operations per second (FLOPS). The network links that connect these hosts together have bandwidth and latency properties. Bandwidth denotes the time it takes for some fixed amount of data to be pushed onto a network link before it can start traveling through the network. Latency denotes the amount of time it will take for that data to traverse through the link before it reaches its destination. The following image illustrates the hardware infrastructure described by the `platform.xml` file *provided in this activity.*

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/platform.svg">Platform</object>

Once the simulated hardware infrastructure has been established through the `platform.xml` file, [WRENCH](http://wrench-project.org/) "user" API can then be used to instantiate various software infrastructures, thus forming a desired "cyberinfrastructure" on which the WMS can run. Figure 1.6 illustrates the simulated cyberinfrastructure devloped for this activity. The *storage service* stores input and output files used by the workflow tasks. A *file registry service* keeps track of what files are located on which storage services (only one in this example). The *WMS* executes tasks in the workflow by sending them as jobs to a *multihost multicore compute service*. A multihost multicore compute service executes the jobs it receives on its compute resources, which may be distributed amongst a number of physical host machines. In this example, 
the multihost multicore compute service has a single compute resource located on the physical machine, *Jupiter*.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/simulation.svg"> Simulation </object>

Now that the [WRENCH](http://wrench-project.org/) simulation components have been discussed, the next step is to run the simulation using the workflow illustrated in Figure 1.4 on the cyberinfrastructure illustrated in Figure 1.6.

## Running the simulation

In a terminal, run the following commands. Step 3 will run the simulation and display simulation output to your terminal window. 

1. `docker pull wrenchproject/wrench-pedagogic-modules:wrench-1.0`
2. `docker pull wrenchproject/wrench-pedagogic-modules:activity-1`
3. `docker container run wrenchproject/wrench-pedagogic-modules:activity-1`

## Interpret the results
.
.
.
WIP, saving here and will add more 

