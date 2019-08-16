
#### Learning objectives:

  - Understand the concept of contention

  - Be able to estimate data transfer times in the presence of contention
  

---


#### Shared Networks

Typically, several data transfers are occurring concurrently (at the same
time) on a network topologies, and these transfers may be using the same
network links. For instance, two concurrent transfers could be along two
routes that share a single link. As a result, a data transfer's performance
can be impacted by other data transfers. When a data transfer goes slower
than it would go if alone in the network, it is because of *contention*
(i.e., competition) for the bandwidth of one or more network link.

#### A Simple Example of Contention

Consider the following topology with the two depicted data transfers, that
each were started at exactly the same time and transfer 100 MB of data
(symbolized by the red and the green arrow).

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_networking/topology_contention.svg">topology with contention</object>
<b>Figure 1:</b> A simple example in which two data transfers contend for bandwidth.

If the green data transfer was by itself, its bandwidth would be 20 MB/sec.
If the red data transfer was by itself, its bandwidth would also be 20
MB/sec. For both transfers the link into host C is the *bottleneck* link.
Therefore, when both transfer happen at the same time, they experience
contention for that bottleneck link. This means that they *share the
bandwidth* of that link. This splitting is fair, meaning that they both
receive half of the link's bandwidth. (It turns out that bandwidth sharing
is a bit complicated in practice as it also depends on latencies, but in
this case both transfers have the same end-to-end latencies, which leads to
fair sharing - see a networking course for more details).

Given the above, both transfers proceed at 10MB/sec, half the bandwidth they would
receive if alone in the network, and thus both complete in time:

$$
T = 200\;us + \frac{100 MB}{10 MB/sec} = 10.0002\;sec
$$


#### An example that's a bit more complicated

Consider now the same scenario as above, with the only difference that now the "red" transfer only transfers 50MB:

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/primer_on_networking/topology_contention_different_sizes.svg">topology with contention and different transfer sizes</object>
<b>Figure 2:</b> A slightly more complex example in which one transfer transfers less data than the other.

In this scenario there are two phases:

  1. In the first phase both transfers proceed with a bandwidth of 10 MB/sec due to contention;
  2. In the second phase, after the "red" transfer has completed, the "green" transfer proceeds alone with a bandwidth of 20 MB/sec.

Therefore, the "red" transfer completes in:

$$
T_{red} = 200\;us + \frac{50\;MB}{10\;MB/sec} = 5.0002\;sec
$$

The "green" transfer transfers its first 50 MB of data with a bandwidth of 10 MB/sec and its last 50 MB of data with
a bandwidth of 20 MB/sec. Therefore, it completes in time:

$$
T_{green} = 200\;us + \frac{50\;MB}{10\;MB/sec} + \frac{50\;MB}{20\;MB/sec} = 7.5002\;sec
$$


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
    - `center_link_bandwidth`: the bandwidth of the middle link (in MB/sec), in the range [1, 1000]

For example, the command `docker container run wrenchproject/wrench-pedagogic-modules:activity-0 2 75 100` will simulate
scenario 3 and print the following output to your screen:

```
------------------------------------
         Simulation Results
------------------------------------
       files transferred: 2
               file size: 75 MB
            min duration: 1.582861
            max duration: 1.5867855
           mean duration: 1.5848232
coefficient of variation: 0.124%
```

Notice that some statistics are provided about the file transfers.
This is because when transferring more than one file concurrently, the
file transfer times
may differ slightly. Additionally, these file transfers
do not start at the exact same moment, nor do they end at the exact same
moment. These are minute details captured by the simulation, which we do
not model in the estimate equations above.

### Conclusion

Throughout the upcoming activities, you will be asked to estimate
application execution times given specific hardware constraints. 
Estimates as above will be sufficient to answer all questions but will not be 100%
correct. We will be using simulation to verify that the estimates are close
to real-world values. Continue to the next section, [A Primer on Workflow
Executions]({{ site.baseurl
}}/activities/primer_on_workflow_executions), where we will examine the sequence
of events that make up a simple workflow execution.
