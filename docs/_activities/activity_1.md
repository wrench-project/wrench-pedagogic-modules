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

<!-- data vis content inline for now  -->

<style>
#chart {
    width: 600px;
    height: 500px;
    background-color: white;
}

.x-axis path .x-axis line {
    shape-rendering: crispEdges;
}

.x-axis text {
    font-family: sans-serif;
}

.caption .figure-header {
    font-size: 20px;
}

.caption .content {
    font-size: 14px;
    fill: #666666;
}

.tooltip-item {
    margin: auto;
}

#tooltip {
    position: absolute;
    width: auto;
    padding: 10px;
    background-color: #e8e7e5;
    border-radius: 5px;
    border: 0px solid #000;
    pointer-events: none;
    display: none;
    font-family: sans-serif;
    font-size: 12px;
}
</style>

<script src="{{ site.baseurl }}/public/scripts/d3.min.js"></script>

<div id="chart" class="figure">
    <div id="tooltip">
        <p class="tooltip-item" id="task-id"></p>
        <p class="tooltip-item" id="duration"></p>
    </div>

</div>

<script>
var tasks = [
    {'id': 'task0', 'start': 0.00130987, 'end': 33814, 'host': 'Jupiter'},
    {'id': 'task1', 'start': 33814, 'end': 56175, 'host': 'Jupiter'},
    {'id': 'task2', 'start': 56175, 'end': 76053, 'host': 'Jupiter'},
    {'id': 'task3', 'start': 76053, 'end': 96156, 'host': 'Jupiter'},
    {'id': 'task4', 'start': 96156, 'end': 125968, 'host': 'Jupiter'},
];

// Chart params
var chart_height = 400;
var chart_width = 600;
var svg_height = 500;
var padding = 60;


// Create the svg element
var svg = d3.select('#chart')
    .append('svg')
    .attr('width', chart_width)
    .attr('height', svg_height);

// Create scales
var x_scale = d3.scaleLinear()
    .domain([0, d3.max(tasks, function(d) {
        return d['end'];
    })])
    .range([padding, chart_width - padding]);

/*var y_scale = d3.scaleLinear()
    .domain([0, d3.max(tasks, function(d) {
        return d['host'];
    })])
    .range([chart_height - padding, padding]);*/

var hosts = [];
tasks.forEach(function(task) {
    hosts.push(task['host']);
})

var band_scale = d3.scaleBand()
    .domain(hosts)
    .range([chart_height - padding, padding])
    .padding(0.1);

// Add the data
svg.selectAll('rect')
    .data(tasks)
    .enter()
    .append('rect')
    .attr('x', function(d) {
        return x_scale(d['start']);
    })
    .attr('y', function(d) {
        return band_scale(d['host']);
    })
    .attr('width', function(d) {
        return x_scale(d['end']) - x_scale(d['start']);
    })
    .attr('height', band_scale.bandwidth())
    .attr('fill', '#ffe1bf')
    .attr('stroke', 'grey')
    .attr('stroke-width', 1)
    .on('mouseover', function() {
        d3.select(this)
            .attr('fill', '#ffc268');

        d3.select('#tooltip')
            .style('display', 'inline');
    })
    .on('mousemove', function(d) {

        d3.select('#tooltip')
            .style('left', (d3.event.pageX) + 'px')
            .style('top',  (d3.event.pageY - 60) + 'px');

        d3.select('#task-id').text('TaskID: ' + d['id']);
        d3.select('#duration').text('Duration: ' + (d['end'] - d['start']));
    })
    .on('mouseout', function() {
        d3.select('#tooltip')
            .style('display', 'none');

        d3.select(this)
            .transition()
            .attr('fill', '#ffe1bf');
    });


// Create axis
var x_axis = d3.axisBottom(x_scale);
svg.append('g')
    .attr('class', 'x-axis')
    .attr('transform',
        'translate(0,' + (chart_height - padding) + ')')
    .call(x_axis);

var y_axis = d3.axisLeft(band_scale);
svg.append('g')
    .attr('class', 'y-axis')
    .attr('transform',
        'translate(' + padding + ',0)')
    .call(y_axis);

// Create x-axis label
svg.append('text')
    .attr('transform',
        'translate(' + (chart_width / 2) + ' ,' + (chart_height - 10) + ')')
    .style('text-anchor', 'middle')
    .attr('font-size', 12+'px')
    .text('Time (seconds)');

// Create y-axis label
svg.append('text')
    .attr('transform',
        'rotate(-90)')
    .attr('y', -3)
    .attr('x', 0 - (chart_height / 2))
    .attr('dy', '1em')
    .attr('text-anchor', 'middle')
    .attr('font-size', 12+'px')
    .text('Execution Host');

// Create Caption
svg.append('g')
    .attr('class', 'caption')
    .attr('transform',
        'translate(0,' + (chart_height + 10) + ')')
    .append('text')
    .attr('class', 'figure-header')
    .text('Figure 1.7');

svg.select('.caption')
    .append('text')
    .attr('class', 'content')
    .attr('y', function() {
        return d3.select(this.parentNode).attr('y') + 30;
    })
    .text('The execution of tasks on each host throughout the course of the simulation.');

</script>

<!-- end data vis content  -->

Figure 1.7 above illustrates when workflow tasks in this simulation are executed by the multihost multicore compute service (mouse over each bar for more information).
Based on the DAG representation of our workflow in Figure 1.4, we can see that 'task 1', 'task 2', and 'task 3' can all be executed concurrently (given the
necessary physical resources). This platform has a limited number of resources, however a platform more representative of modern infrastructures might have
multiple compute hosts, each with multiple cores that may be used to perform tasks concurrently. More on this will be explored in the next section:
*Activity 2: Parallelism*.

