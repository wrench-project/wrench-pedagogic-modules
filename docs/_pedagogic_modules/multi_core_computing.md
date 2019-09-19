---
layout: page
title: 'B. Multi-core Computing'
order: 200
usemathjax: true
---

The goal of this module is to introduce you to the basic concepts of running 
multiple instances of single-threaded programs on a multi-core computer.


#### Learning Objectives:

  - Understand the need for using multi-core machines;

  - Understand the concepts of parallel speedup and efficiency;

  - Understand the relationship between idle time, speedup and efficiency;
  
  - Understand the impact of RAM constraints on idle time.

----

<!--
# Multi-Core Computing

1. [Basics](#basics)
2. [Benefits](#benefits)
    * [Speedup](#speedup)
    * [Efficiency](#efficiency)
3. [Exercises](#exercises)
-->

## Basics

Multi-core processors have become ubiquitous due to well-documented issues
with further increasing the performance of a single core.  Each core is
capable of executing code independently of other cores. 
In this module, we take a simplified view of multi-core machines.  We
consider only programs that can use a single core, a.k.a.  single-threaded
programs, and will take a look at  multi-threaded programs later.

We use the term *tasks* to refer to independent executions of one program
but for different input. For instance, we could have 5 tasks where each
task renders a different frame of a movie. Of course, we could also have
each task correspond to a different program, which will be the topic of an
upcoming module. We call the set of tasks we want to execute an **application**.

As mentioned in the [Single Core Computing]({{ site.baseurl
}}/pedagogic_modules/single_core_computing) module, we do not consider time
sharing. That is, we will only consider application executions in which at most one
task runs on a core at a given time. Although the Operating System allows
time-sharing, we will simply never start more tasks than cores on the
machine.  Therefore, a task that begins running on a core will run
uninterrupted until completion on that same core.


## Parallelism

The motivation for running the tasks of an application on multiple cores is speed.  For
example, if you have tasks that a single core can complete in one hour, it
will take four hours to complete four tasks. If you have a two of these
cores in a dual-core processor, now you can complete the same four tasks in
two hours only. This concept is called **parallelism**: running multiple 
tasks at the same time, or *concurrently*, to complete a set of tasks faster.

Unfortunately, most real-world applications do not have the ideal behavior
above, that is, execute *n* times faster with *n* cores. Instead, they execute
less than *n* times faster. This may seem surprising, but comes about due to 
many reasons. In this module, we'll see one of these reasons. 

To explore parallelism, we first have to define two interesting metrics:
*Parallel Speedup* (or *Speedup*) and *Parallel Efficiency* (or
*Efficiency*).

### Parallel Speedup

The speedup is a metric used to quantify the acceleration in speed due to
using multiple cores.  It is calculated by dividing the execution time of
the application on a single core by the execution time of the application on
multiple cores, say *p*. The speedup on *p* cores is thus expressed as
follows:

$$
\begin{align}
\text{Speedup_{p}} & = \frac{\text{Execution Time}_{1}}{\text{Execution Time}_{p}}
\end{align}     
$$

For instance, if an application runs in 3 hours on 1 core but runs in 2 hours on 2 cores, then its speedup is:
$$
\begin{align}
\text{Speedup_{2}} & = \frac{3}{2} = 1.5\;.
\end{align}     
$$

In this example, we would be somewhat "unhappy" because although we have 2 cores, but *only* go 1.5 times faster. This leads us to our next metric!


### Parallel Efficiency

The concept of efficiency is essentially about how much useful work the
cores can do for an application, or how much "bang" do you get for your
"buck". The "bang" is the speedup, and the "buck" is the number of cores.
More formally, the efficiency of an application execution on $p$ cores is: 

$$
\begin{align}
\text{Efficiency_p} & = \frac{\text{Speedup_p}}{\text{p}}\;.
\end{align}     
$$

If the speedup going from 1 core to 2 cores is 1.5, then the
efficiency is:

$$
\begin{align}
\text{Efficiency_2} & = \frac{1.5}{\text{2}} & = 0.75 = 75%\;.
\end{align}     
$$

In the best case, the efficiency would be 100\% (which corresponds to going
*p* times faster with *p* cores). In the above example, it is only 75%, meaning
that we are "wasting" some of the overall compute capacity during the application's 
execution. 

At this point, you may be wondering, how is this (less than 100% efficiency) possible?

## Load Imbalance and Idle Time

        - p does not divide n (p is greater than n)
        - "the load is not balanced"

        - RUN Simulation
        - Questions
        - Practice Questions

## Adding RAM constraints

        - RUN Simulation
        - Questions
        - Practice Questions

-- BARF ---

When looking at sequential computing efficiency is a simple concept, the processor is either working a task or the 
program has completed. When looking at multi-core computing however, the notion of efficiency becomes important as part 
of the processor (some cores) may be working while others sit idle, and doubling the number of cores usually will not 
double the speed.  


## Exercises

These exercises accompany the simulator "Multi-core Machines". You will need to do calculations similar to those done in
the sequential computing section, but now it will be the number of cores and tasks that vary, rather than the flops. 

##### For These Questions, We Will Assume The Tasks Do Not Require RAM, It Can Remain At 0. 

**[q1]** What is the speedup that you will observe for running ten tasks on one core versus ten tasks on two cores? What
is the efficiency?

**[q2]** What is the speedup that you will observe for running ten tasks on one core versus ten tasks on three cores? 
What is the efficiency?

##### For These Questions, RAM Will Vary

**[q3]** The host we are using has 32GB of ram, how long does it take to run two tasks on two cores if they each require
16GB of RAM? What if they require 20GB of RAM each?

**[q4]** Figure out what happens for the scenario described in Question 1, but this time the tasks are 
using 16GB of RAM. What are the speedup and efficiency now?

