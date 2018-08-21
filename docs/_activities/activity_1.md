---
layout: page
title: 'Activity 1: Running Your First Simulated Workflow Execution'
---

1. [Learning objectives](#learning-objectives)
2. [Running the simulation](#running-the-simulation)
3. [Interpreting the results](#interpret-the-results)

# Overview

## Learning Objectives

  - Learn how to run WRENCH simulations from Docker containers
  - Learn how to understand a workflow execution timeline and seauence of important execution event
  - Understand how I/O overhead impacts overall workflow execution performance
  - Gain exposure to the problem of data locality

## Workflow and Platform Scenario

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/workflow.svg">Workflow</object>

In this activity, we study the execution of the workflow depicted in Figure 1.1 on the cyber infrastructure depicted in
Figure 1.2. A Compute Service (CS) does exactly as its name implies and will execute tasks that the Workflow Management System
sends to it. The Storage Service stores files, much like a database, and handles read and write requests.
 This is a very simple scenario, and will be used to get our "feet wet" with WRENCH simulations.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_1/cyber_infrastructure.svg">Cyber Infrastructure</object>

## WMS Scenario

We execute the workflow with a (already implemented) WMS that greedily submits tasks
to the Compute Service as soon as they become ready. Each task running on the CS reads and writes data
from/to the Storage Service (which, from the perspective of the task is on a remote host).
Once the WMS is notified by the CS that a task has completed, it will greedily submit the next ready task.
This is repeated until all tasks have completed.

# Activity Steps

## Step #1: Run the Simulation

In a terminal, run the following commands.

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-1`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-1`

Step 2 will display a lot of textual simulation output to your terminal window. This output indicates
(simulated) actions and events throughout the execution of the workflow.

## Step #2: Interpret the Workflow Execution

<div class="wrench-output">
<span style="font-weight:bold;color:rgb(187,0,187)">[0.000000][Tremblay:wms_activity1_3(1)] Starting on host Tremblay listening on mailbox_name wms_activity1_3<br></span>
<span style="font-weight:bold;color:rgb(187,0,187)">[0.000000][Tremblay:wms_activity1_3(1)] About to execute a workflow with 5 tasks<br></span>
<span style="font-weight:bold;color:rgb(0,0,187)">[0.000000][Tremblay:wms_activity1_3(1)] Submitting task0 as a job to compute service on Jupiter<br></span>
<span style="font-weight:bold;color:rgb(187,0,0)">[33814.005184][Tremblay:wms_activity1_3(1)] Notified that task0 has completed<br></span>
<span style="font-weight:bold;color:rgb(0,0,187)">[33814.005184][Tremblay:wms_activity1_3(1)] Submitting task1 as a job to compute service on Jupiter<br></span>
<span>.<br></span>
<span>.<br></span>
<span>.<br></span>
</div>

The simulation will produce output similar to the above snippet of text. The first column denotes the simulation time at which some process is performing some action.
The second column denotes the process name and the host that it is running on. Last is a message describing what the process is doing. For example, the second line from the output
above, `[0.000000][Tremblay:wms_activity1_3(1)] About to execute a workflow with 5 tasks` tells us
that at *simulation time 0.00000*, the WMS named *activity1*, located on the physical host, *Tremblay*, is *"About to execute a workflow with 5 tasks"*.
Simulation output for this activity has been constrained such that only messages from the WMS are visible. Furthermore, the color scheme of the
output has been set up such that general messages are <span style="font-weight:bold;color:rgb(187,0,187)">pink</span>, task submissions to the CS are
<span style="font-weight:bold;color:rgb(0,0,187)">blue</span>, and notifications received from the CS are
<span style="font-weight:bold;color:rgb(187,0,0)">red</span>.

**Questions**
  - At what time did the WMS submit *task1* as a job to the compute service?
  - At what time did the WMS receive a notification that *task1* has completed?
  - The compute service runs on a host with a speed of *1000Gf*. *task4* takes *100Tf* to compute. About
  how long should *task4* compute for?
  - Based on the simulation output, how long does it take for *task4* to complete?
  - Why does *task4* take longer than what you computed in *question 3*?
  - Assuming there is no traffic on the network, about how long would it take to send a
  *250 megabyte file* from *Jupiter* to *Fafard* using the direct link between the two hosts?

## Step #3: Visualize the Workflow Execution

Analyzing the simulation output can be tedious, especially when the workflow comprises
a large number of tasks or when there are a number of complex software services being
simulated. Fortunately, the simulator can produce a visualization of the workflow execution
as a Gantt chart.

In the terminal run the following commands:
1. `some command`
2. `another command`
3. open a browser and go to localhost:3000

**Questions**
  - What fraction of *task0* is spent doing I/O?
  - What fraction of *task4* is spent doing I/O?
  - Double the link bandwidth. Now, what fraction of *task4* is spent doing I/O?
  - With the link bandwidth doubled, how much faster is the workflow execution now than before?

## Step #4: Better Data locality with another storage service on the compute host

The compute service is reading and writing files from and to a remote storage service, thus contributing
to the I/O overhead. This can be avoided if the storage service were to be located on the same host as
the compute service or vice versa. Here we introduce the idea of moving computations closer to where
data resides, or **data locality**.


To simulate this, select `some button that says storage service on same host as cs...`.

**Questions**
  - What fraction of *task4* is spent doing I/O?
  - How much faster is the workflow execution now than


## Figures disappear, and students have to run stuff to see them


[WRENCH](http://wrench-project.org/) keeps track of simulation events as they occur so that users may analyze areas of interest. In this activity, we are
interested in observing how the WMS
executed our workflow from Figure 1.4. The start and completion times of each task have been plotted in Figure 1.7.

<style>
#chart1 {
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

#tooltip1 {
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

<div id="chart1" class="figure">
    <div id="tooltip1">
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
var svg = d3.select('#chart1')
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

        d3.select('#tooltip1')
            .style('display', 'inline');
    })
    .on('mousemove', function(d) {

        d3.select('#tooltip1')
            .style('left', (d3.event.pageX) + 'px')
            .style('top',  (d3.event.pageY - 60) + 'px');

        d3.select('#task-id').text('TaskID: ' + d['id']);
        d3.select('#duration').text('Duration: ' + (d['end'] - d['start']));
    })
    .on('mouseout', function() {
        d3.select('#tooltip1')
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

Figure 1.7 above illustrates when workflow tasks in this simulation are executed by the multihost multicore compute service (mouse over each bar for more information).
Based on the DAG representation of our workflow in Figure 1.4, we can see that 'task 1', 'task 2', and 'task 3' can all be executed concurrently (given the
necessary resources). Modifying our simulation such that the multihost multicore compute service has three compute resources (one on each host) causes the simulation to run differently. The addition of a few more compute resources reduces the overall workflow execution time as depicted in Figure 1.8 below.  

<style>
#chart2 {
    width: 600px;
    height: 500px;
    background-color: white;
}

#tooltip2 {
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

<div id="chart2" class="figure">
    <div id="tooltip2">
        <p class="tooltip-item" id="task-id2"></p>
        <p class="tooltip-item" id="duration2"></p>
    </div>
</div>


<script>

tasks = [
    {'id': 'task0', 'start': 0.00130987, 'end': 33814, 'host': 'Fafard'},
    {'id': 'task1', 'start': 33814, 'end': 56175, 'host': 'Fafard'},
    {'id': 'task2', 'start': 33814, 'end': 53692, 'host': 'Jupiter'},
    {'id': 'task3', 'start': 33814, 'end': 53917, 'host': 'Tremblay'},
    {'id': 'task4', 'start': 56175, 'end': 85987, 'host': 'Fafard'},
];

// Chart params
var chart_height = 400;
var chart_width = 600;
var svg_height = 500;
var padding = 60;


// Create the svg element
var svg = d3.select('#chart2')
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

        d3.select('#tooltip2')
            .style('display', 'inline');
    })
    .on('mousemove', function(d) {

        d3.select('#tooltip2')
            .style('left', (d3.event.pageX) + 'px')
            .style('top',  (d3.event.pageY - 60) + 'px');

        d3.select('#task-id2').text('TaskID: ' + d['id']);
        d3.select('#duration2').text('Duration: ' + (d['end'] - d['start']));
    })
    .on('mouseout', function() {
        d3.select('#tooltip2')
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
    .text('Figure 1.8');

svg.select('.caption')
    .append('text')
    .attr('class', 'content')
    .attr('y', function() {
        return d3.select(this.parentNode).attr('y') + 30;
    })
    .text('The execution of tasks on each host throughout the course of the simulation.');

</script>

Initially, the simulation ran for 125978 simulated seconds, however the addition of several compute resources reduced
the execution time to 85987 simulated seconds, roughly a *32% decrease*. Workflow executions can benefit from parallelism,
however there are limitations. More on this will be explored next in *Activity 2: Parallelism*.
