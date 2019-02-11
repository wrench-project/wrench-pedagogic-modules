---
layout: page
title: 'A Brief Primer On File Transfer Times'
order: 1
usemathjax: true
---

### Overview

When going through these pedagogic modules, you will need to estimate
durations of data transfer times,  which is something you may not have
done previously. Back-of-the-envelope estimates are not difficult to
compute.
For instance, sending 100MB of data over a network link with a
10MB/s bandwidth and a 0.001s latency would be estimated to take
10.001 seconds. Real-world networks exhibit several hardware and software
effects that are not captured by this estimate.
In the upcoming pedagogic module we do not use real-world
networks and instead we simulate them. But simulations are done using
[WRENCH](http://wrench-project.org/), which is based on the
[SimGrid](http://simgrid.org) simulation framework, which implements
realistic simulation models that do capture many real-world network
effects. So, in our simulations, sending 100MB of data over a network link
with a 10MB/s bandwidth and a 0.001s latency does not take 10.001
seconds (it takes longer, as it would in real-world networks).  When going
through these pedagogic modules and inspecting execution timelines, you
will thus note that your back-of-the-envelope calculations of data transfer
times, which are sufficient to answer all questions, do not exactly align
with the simulation. In other words, it is normal to see some discrepancies
between your estimates and what you observe in simulation.

To make sure you are able to estimate file transfer times and to
demonstrate discrepancies between estimated and simulated times, we present
below three simple scenarios where files need to be sent from one host to
another. For each scenario we explain how data transfer times can be
estimated.

### Scenario 1: A Single file transfer

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_file_transfer_times/scenario_1.svg">Scenario 1</object>

*About how long should it take to send a single 100 MB file from "host1" to "host2"?*
 The time $T$ it takes to send a file of size $m$ over a single link with a bandwidth $\beta$ and latency $\alpha$,
can be estimate as follows:

$$ T = \alpha + \frac{m}{\beta} .$$

In this scenario, the file transfer goes through a route consisting of 3 network links.
We thus should modify the above equation.
Let $l$ be a link on the route $r$ over which the file is being sent.
Then we can estimate, naively, the file transfer time with the following:

$$
\begin{align}

 T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{100MBps} \\
  & = 3(0.0001s) + \frac{100MB}{100MBps} \\
  & = 1.0003s.

\end{align}
$$

This is correct because all 3 links have the same bandwidth, i.e., our estimate
assumes there is a single link with a latency that is the sum of the latencies.
Using [WRENCH](http://wrench-project.org/) to simulate this scenario, we
would observe a file transfer time of 1.098119 seconds.
As previously mentioned, [SimGrid](http://simgrid.org) models several network overheads
and network protocol (TCP) effects. In fact, due to network data overheads,
only 92% of the maximum physical bandwidth of a link can be used for the
data transfer.
We can account for this phenomenon by modifying
our equation to be:

$$
\begin{align}

T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{0.92(100MBps)} \\
  & = 3(0.0001s) + \frac{100MB}{0.92(100MBps)} \\
  & = 1.0872s

\end{align}
$$

This estimate is much closer to what is observed in simulation. However, a
discrepancy still exists, because our estimate does not capture many
other network effects, e.g., the famous TCP "slow start" behavior (see a
networking course). Attempting to estimate the data transfer time more
precisely would be difficult (which is why ultimately one must rely on
simulation). But, in all the upcoming pedagogic modules, you can use either
of the two above equations to obtain useful, but somewhat inaccurate,
estimates of data transfer times.


### Scenario 2: A bottleneck link

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_file_transfer_times/scenario_2.svg">Scenario 2</object>

*About how long should it take to send a single 100 MB file from "host1" to "host2" given that the middle network link now
has a bandwidth of only 10MBps?* It is almost always the case in practice that data will be transmitted over a heterogeneous set of
network links. Along a route, the data transfer rate is bounded by the link with the
smallest bandwidth, or the *bottleneck link*. In this scenario, the
bottleneck link is the middle link, which has a bandwidth of 10MBps. We
can thus modify the estimate for the previous scenario by using a $min$
operator:

$$
\begin{align}

T_{1file} & = \sum_{l \in r} Latency(l) + \frac{m}{0.92 (\min\limits_{l \in r} Bandwidth(l))} \\
  & = 3(0.0001s) + \frac{100MB}{0.92(10MBps)} \\
  & = 10.8698

\end{align}
$$

Simulation results for this scenario show that the date transfer would in fact take 10.8406 seconds. There is thus, here again, a small discrepancy.

### Scenario 3: Two concurrent file transfers

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_file_transfer_times/scenario_3.svg">Scenario 3</object>

*About how long should it take to send two 75 MB files concurrently from "host1" to "host2"?* In this situation,
the bandwidth will be shared amongst the two concurrent file transfers, which are assumed to start at the same time (and will thus terminate at about the same time). Let $n$ be the number of files to send
concurrently. Building off of the previous equation, we have the following:

$$
\begin{align}

T_{2files} & = \sum_{l \in r} Latency(l) + \frac{nm}{0.92 (\min\limits_{l \in r} Bandwidth(l))} \\
  & = 3(0.0001s) + \frac{2 * 75MB}{0.92(100MBps)} \\
  & = 1.63073s

\end{align}
$$

Based on the simulation results, the 2 concurrent file transfers should
take about 1.6335 seconds. Again, the estimate is close, but does not align
perfectly with the simulation.

### Try the file transfer simulation

We provide you with the simulator used to obtain the above simulation
results, in case you want to experiment yourselves with different
latencies, bandwidths, and numbers of concurrent file transfers, and check whether
your computed estimates are roughly accurate. In a
terminal, run the following commands.

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-0`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 <num_files> <file_size> <center_link_bandwidth>`
    - `num_files`: the number of files to transfer, in the range [1, 100]
    - `file_size`: the size of each file (in MB), in the range [1, 1000]
    - `center_link_bandwidth`: the bandwidth of the middle link (in MBps), in the range [1, 1000]

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

Notice that some statistics are provided about the file transfers.
This is because when transferring more than one file concurrently, the
file transfer times
may differ slightly. Additionally, these file transfers
do not start at the exact same moment, nor do they end at the exact same
moment. These are minute details captured by the simulation, which we do
not model in the estimate equations above.

### Conclusion

Throughout the upcoming pedagogic modules, you will be asked to estimate
application execution times given specific hardware constraints. Your
estimates will be sufficient to answer all questions but will not be 100%
correct. We will be using simulation to verify that the estimates are close
to real-world values. Continue to the next section, [Activity 1: Running
Your First Simulated Workflow Execution]({{ site.baseurl
}}/activities/activity_1), where we will begin running basic
[WRENCH](http://wrench-project.org/) simulations.
