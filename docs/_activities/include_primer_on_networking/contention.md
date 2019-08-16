
#### Learning objectives:

  - Understand the concept of contention

  - Be able to estimate data transfer times in the presence of contention
  

---


#### Networks are shared

Typically, several data transfers are occurring concurrently (at the same
time) on a network topologies, and these transfers may be using the same
network links. For instance, two concurrent transfers could be along two
routes that share a single link. As a result, a data transfer's performance
can be impacted by other data transfers. When a data transfer goes slower
than it would go if alone in the network, it is because of *contention*
(i.e., competition) for the bandwidth of one or more network link.

#### A Simple example

Consider the following topology with the two depicted data transfers, that
each were started at exactly the same time and transfer 100 MB of data
(symbolized by the red and the green arrow).

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_networking/topology_contention.svg">topology with contention</object>
<b>Figure 1:</b> A simple example in which two data transfers contend for bandwidth.

If the green data transfer was by itself, its bandwidth would be 30 MB/sec.
If the red data transfer was by itself, its bandwidth would be 40
MB/sec. But when both transfer happen at the same time, they experience
contention on the link into host C. This means that they *share the
bandwidth* of that link. This sharing is fair, meaning that they both
receive half of the link's bandwidth, 20 MB/sec. (It turns out that bandwidth sharing
is a bit complicated in practice as it also depends on latencies, but in
this case both transfers have the same end-to-end latencies, which leads to
fair sharing - see a networking course for more details).

Given the above, both transfers proceed at 10MB/sec, i.e., half the bandwidth of the link into
host C, which is their bottleneck link. 
Thus both transfers complete in time:

$$
T = 200\;us + \frac{100 MB}{20 MB/sec} = 5.0002\;sec
$$


#### A slightly more complex example

Consider now another scenario, with the only difference that now the "red" transfer only transfers 50MB:

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_networking/topology_contention_different_sizes.svg">topology with contention and different transfer sizes</object>
<b>Figure 2:</b> A slightly more complex example in which one transfer transfers less data than the other.

In this scenario there are two phases:

  1. In the first phase both transfers proceed with a bandwidth of 20 MB/sec due to contention;
  2. In the second phase, after the "red" transfer has completed, the "green" transfer proceeds alone with a bandwidth of 30 MB/sec (it's bottleneck link is now the link out of host B!).

Therefore, the "red" transfer completes in:

$$
T_{red} = 200\;us + \frac{50\;MB}{20\;MB/sec} = 2.5002\;sec
$$

The "green" transfer transfers its first 50 MB of data with a bandwidth of 20 MB/sec and its last 50 MB of data with
a bandwidth of 30 MB/sec. Therefore, it completes in time:

$$
T_{green} = 200\;us + \frac{50\;MB}{20\;MB/sec} + \frac{50\;MB}{30\;MB/sec} = 4.1668\;sec
$$

#### Testing your understanding using simulation

In many of the upcoming pedagogic activities on this site you have to
run experiments using simulation. We provide you here with a simple simulator
for the following scenario in which an number of transfers occur concurrently on the
same three-link route:

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_networking/topology_contention_simulation.svg">simulation scenario</object>
<b>Figure 3:</b> Simulation scenario.

With the simulator you can experiment with any scenario, so as to experimentally test your
understanding of contention.  In a terminal, do the following:

1. Install the simulator: `docker pull wrenchproject/wrench-pedagogic-modules:activity-0`
2. Run the simulator: `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 <file size> <file size> ...`
    - Each `file size` argument is the data size (in MB) transferred by one of the concurrent data transfers

For example, the command `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 100` will simulate a single 10MB data transfer and produce this output:

```
----------------------------------------
           Simulation Results
----------------------------------------
100 MB transfer completed at time 10.5
----------------------------------------
```

Note that the transfer's completion time is a bit higher than what the computations
we've done so far. We would expect the transfer time to be:

$$
T = 30\;us + \frac{100 MB}{10 MB} = 10.00003\;sec.
$$

This discrepancy is due to the simulator capturing some details of
real-world networks (e.g., the TCP slow-start behavior that you may have
heard of in a Networking course). These details are not captured by the
above mathematical expression. This said, such mathematical expressions are
still useful approximation that we can use to reason about data transfer
times. However, we should not be surprised to find that our calculations
are a bit "off" when compared to simulation results.

Another execution of the simulator as `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 100 50 100` will simulate two 100MB transfers and one 50MB transfer, producing this output:

```
----------------------------------------
           Simulation Results
----------------------------------------
100 MB transfer completed at time 26.25
50 MB transfer completed at time 15.75
100 MB transfer completed at time 26.25
----------------------------------------
```

As expected, the 50 MB transfer completes first, and the two 100MB transfers
complete at the same time.

Feel free to run simulations to explore different scenarios and test your 
data transfer times for various combinations of concurrent transfers.

--- 

#### Practice questions

The following practice questions pertain to this topology:

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_networking/topology_contention_practice.svg">simulation scenario for practice questions</object>
<b>Figure 4:</b> Topology for practice questions.


**[q1]** TBD
<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   TBD
  </div>
</div>

<p> </p>


**[q2]** TBD
<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   TBD
  </div>
</div>

<p> </p>


**[q3]** TBD
<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   TBD
  </div>
</div>

<p> </p>



--- 

Continue on to the next section, [A Primer on Workflow
Executions]({{ site.baseurl
}}/activities/primer_on_workflow_executions), where we will examine the sequence
of events that make up a simple workflow execution.


