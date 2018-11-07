---
layout: page
title: 'A brief primer on file transfer times'
order: 1
usemathjax: true
---

### Overview

When going through these pedagogic modules, you will need to do
back-of-the-envelope calculations of durations of computation and of file
transfer times.  While estimating computation times is straightforward,
estimating data transfer times is a bit more involved and may be something
to which you have not been exposed previously. Furthermore,
[WRENCH](http://wrench-project.org/) is based on the
[SimGrid](http://simgrid.org) simulation framework.
[SimGrid](http://simgrid.org) strives to implement realistic simulation
models that capture many of the effects seen in real-world platforms.  For
instance, sending 100MB of data over a network link with a bandwidth of
1000MB/sec and a latency of 10ms does not take exactly 1.01 seconds on
real-world networks (e.g., due to network protocol effects, due to various
network overheads), which is captured in [SimGrid](http://simgrid.org)
simulations.  When going through these pedagogic modules and inspecting
execution timelines, you will thus likely note that back-of-the-envelope
calculations of durations, e.g., data transfer times, do not always align
with the simulation.

To make sure you are able to estimate file transfer times and to
demonstrate discrepancies between estimated and simulated times, we present
to you three simple scenarios where files need to be sent from one host to
another. Then we explain how you can mathematically model each situation.

### Scenario 1: Single file transfer

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_file_transfer_times/scenario_1.svg">Scenario 1</object>

*About how long should it take to send a single 100 MB file from "host1" to "host2"?*
 The time $T$ it takes to send a file of size $m$ over a single link with a bandwidth $\beta$ and latency $\alpha$,
can be modeled by the following:

$$ T = \alpha + \frac{m}{\beta} $$

In this scenario, the file transfer will travel through a route consisting of 3 network links so we
can modify the equation.
Let $l$ be a link on the route $r$ over which the file is being sent.
Then we can estimate, naively, the file transfer time with the following:

$$
\begin{align}

 T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{100MBps} \\
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

T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{0.92(100MBps)} \\
  & = 3(0.0001s) + \frac{100MB}{0.92(100MBps)} \\
  & = 1.0872s

\end{align}
$$

This calculation is much closer to what was derived from the simulation, however a discrepancy still
exists between our calculation and the simulation. [SimGrid's](http://simgrid.org) rigorous approach
to modeling networks can be difficult to mimic with back-of-the-envelope calculations, especially
in situations where the state of the network is in constant flux due to changes in network traffic.
As such, you can use either of the two equations for rough estimates of file transfer times.


### Scenario 2: Bottleneck links

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_file_transfer_times/scenario_2.svg">Scenario 2</object>

*About how long should it take to send a single 100 MB file from "host1" to "host2" given that the middle network link now
has a bandwidth of 10 MBps?* It is almost always the case that data will be transmitted over a heterogeneous set of
network links, hence the use of the $min$ operation to account for a bottleneck link. In this scenario, the file
transfer rate of our network links are bounded at 10 MBps because of the middle link.

$$
\begin{align}

T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{0.92 (\min\limits_{l \in r} Bandwidth(l))} \\
  & = 3(0.0001s) + \frac{100MB}{0.92(10MBps)} \\
  & = 10.8698

\end{align}
$$

Simulation results for this scenario show that the file transfer would take 10.8406 seconds to complete.

### Scenario 3: Two concurrent file transfers

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_file_transfer_times/scenario_3.svg">Scenario 3</object>

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

### Try the file transfer simulation

We provide the simulation used in this activity. In a terminal, run the following commands.

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-0`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 <num_files> <file_size> <center_link_bandwidth>`
    - `num_files`: the number of files in the range [1, 100]
    - `file_size`: the size of each file (in MB) in the range [1, 1000]
    - `center_link_bandwidth`: the bandwidth of the center link (in MBps) in the range [1, 1000]

For example, the command `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 2 75 100` will simulate
scenario 3 and print the following output to your screen:

```
------------------------------------
         Simulation Results
------------------------------------
       files transferred: 2
               file size: 75 MB
            min duration: 1.6315617
            max duration: 1.6354868
           mean duration: 1.6335243
coefficient of variation: 0.12%
```

Notice that there are some statistics provided about the file transfers. When transferring more than one file
concurrently, the duration it takes to transfer each file may differ slightly. Additionally, these file
transfers do not start at the exact same moment, nor do they end at the exact same moment. These are minute details
captured by the simulation, which we cannot fully model with the equations above.  

### Conclusion

To this end, we have introduced the goal of these pedagogic modules and laid the foundation for the coming
activities. Throughout the course of each activity, you will be asked to estimate file transfer times and task
execution times given a specific set of hardware constraints. Note that your answers will not be exact, and so
we will be using simulations to verify calculations. Continue to the next section,
[Activity 1: Running Your First Simulated Workflow Execution]({{ site.baseurl }}/activities/activity_1),
where we will begin running basic [WRENCH](http://wrench-project.org/) simulations.
