
#### Learning objectives:

  - Understand the concepts of latency and bandwidths

  - Be able to estimate data transfer time through a network link

---

#### Link Latency and Bandwidth

A network is built from **network links** (in the case of wired
networks, these links comprise network cables). 
Each network link has two important characteristics:

  1. **Latency**: the time it takes for one bit of data to travel along the length of the link (measured in seconds)
  2. **Bandwidth**: the maximum number of bits that can be transferred by the link per time unit (measured in byte/second)

<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    One can think of data flowing through a link as water flowing
through a pipe (click to expand)
  </div>
  <div markdown="1" class="ui segment content">
A popular analogy is to think of a link as a vertical physical pipe
that connects a cistern  (on top) to a pool (on the bottom) .  The
latency is the time for one drop of water to travel from the top-end of
the pipe to the other. The bandwidth is how many liters of water can
flow out of the end of the pipe per second.  In this analogy, the
latency is the _length_ of the pipe, and the bandwidth is its _width_.
</div>
</div>

<p> </p>

We assume that links are **bidirectional**, meaning that data can flow in
both directions at the same time (which is unlike water in pipes). 
This model of a network link abstracts away many of the details of
actual network links (and the fact that latencies are also due to routers). 




---

#### Data Transfer Time

Given a network link with latency $\alpha$ and bandwidth $\beta$, the time
$T$ to transfer an amount of data $s$ over the link can be estimate as
follows:

$$ T = \alpha + \frac{m}{\beta} .$$


For instance, consider a link with latency 100 microseconds and effective bandwidth
120 MB/sec, transferring 100KiB of data takes time: 

$$ T = 100 \times 10^{-6} + \frac{100 \times 1024}{120 \times 10^{6}}  \simeq .000953 sec.$$

(Make sure you know your units and use them in a consistent manner!)

In some cases the first term above (the latency) can dominate (i.e., with
small data sizes and/or large bandwidths), or can be negligible (i.e., with
large data sizes and/or small bandwidths).

Here we have used the term, *effective bandwidth*, to denote the maximum
*possible* throughput that a network link is able to achieve. Due to
various network overheads, a network link can have a throughput of at most
about 97% of its advertised physical bandwidth. For instance, if you purchase
a 100 GB/sec physical link,  From this point forward, when we
describe the bandwidth of a network link, we are always describing its
*effective bandwidth*.


---

#### Practice Questions

To make sure the above is crystal clear, answer the following two practice questions:

**[q1]** How long, in milliseconds, does it take to transfer 250 MiB on a network link with latency 500 microseconds and 20 GB/sec bandwidth?
<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   $$ T = 500 / 1000 + 1000 \times 250 \times 1024^2 / (20 * 10^9 / 1000) \simeq 3.60 ms .$$
  </div>
</div>

<p> </p>

**[q2]** How long, in minutes, does it take to transfer 1 GB on a network link with latency 100 microseconds and 520 MiB/sec bandwidth?
<div class="ui accordion fluid">
  <div class=" title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   $$ T = 100 / (60 \times 10^6) + 10^{9} / (520 \times 1024^2 / 60) \simeq 110 min .$$
  </div>
</div>


---

