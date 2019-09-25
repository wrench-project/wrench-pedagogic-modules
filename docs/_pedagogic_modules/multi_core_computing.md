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
\text{Speedup}_p & = \frac{\text{Execution Time}_{1}}{\text{Execution Time}_{p}}\;
\end{align}     
$$

For instance, if an application runs in 3 hours on 1 core but runs in 2 hours on 2 cores, then its speedup is:

$$
\begin{align}
\text{Speedup}_2 & = \frac{3}{2} = 1.5\;
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
\text{Efficiency}_p & = \frac{\text{Speedup}_p}{\text{p}}\
\end{align}     
$$

If the speedup going from 1 core to 2 cores is 1.5, then the
efficiency is:

$$
\begin{align}
\text{Efficiency}_2 & = \frac{1.5}{2} = 0.75 = \text{75%}\
\end{align}     
$$

In the best case, the efficiency would be 100\% (which corresponds to going
*p* times faster with *p* cores). In the above example, it is only 75%, meaning
that we are "wasting" some of the overall compute capacity during the application's 
execution. 

At this point, you may be wondering, how is this (less than 100% efficiency) possible?


-- BARF ---

## Load Imbalance and Idle Time

Idle time is when efficiency is less because some cores are working and some are not. This is frequently going 
to be an issue when *n* the number of tasks is not divisible by *p* the number of cores. For example, if we have 8 tasks
and 5 cores, they will all be busy for the first 5 tasks, but once that has finished, only 3 of the 5 cores will have 
another task to complete, the remaining 2 will sit idle until the remaining 3 finish. In this situation the load is not 
balanced between cores, but with discrete tasks such as these the balance cannot be improved. 



#### Practice Questions

<!--    
        - p does not divide n (p is greater than n)
        - "the load is not balanced"

        - RUN Simulation
        - Questions
        - Practice Questions
-->

 
- What is the speedup of changing from 4 to 6 cores when you have 24 tasks to run? Each task takes a core 1 second to 
finish.

<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   The total execution time when using 4 cores will be 6 seconds. When increasing from 4 cores to 6 cores, now the total
   execution time is 4 seconds. The parallel speedup achieved is 1.5. 

$$
\begin{align}
  \text{Speedup}_6 & = \frac{6\text{ sec}}{4\text{ sec}} = 1.5
\end{align}     
$$

  </div>
</div>


- What is the efficiency of going from 1 to 4 cores when you have 3 tasks to complete? Each task takes a core 1 second 
to finish. 
        
<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   When using only a single core, the 3 tasks will take 3 seconds to complete. When increasing the number of cores to 4,
   the same tasks can now be done in 1 second. Since *p* the number of cores is greater than *n* the number of tasks, we 
   know that it will not be 100% efficiency. 

$$
\begin{align}
  \text{Speedup}_p & = \frac{3}{1} = 3
\end{align}     
$$

$$
\begin{align}
  \text{Efficiency}_4 & = \frac{3}{4} = 0.75 = \text{75% Efficiency}
\end{align}     
$$

  </div>
</div>


- You are upgrading your deprecated single core processor and you have two new multi-core processors to choose from, one 
with 5 cores 
and one with 10 cores. Your only concern is to maximize efficiency. All of the cores are identical. You have 15 tasks 
to run, each taking a core 1 second to complete. Which multi-core processor will provide the higher efficiency.


<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   When using only a single core, the 15 tasks will take 15 seconds to complete. When increasing the number of cores to 
      5,
   the same tasks can now be done in 3 seconds. When increasing the number of cores to 10, the tasks take 2 seconds. What 
   is important to notice is that for the last second, 5 of the cores in the 10 core processor will be idle. Since our main
   concern for this exercise is efficiency rather than raw speed, we would go with the 5 core processor.

$$
\begin{align}
  \text{Speedup}_5 & = \frac{15}{3} = 5\\
  \text{Efficiency}_5 & = \frac{5}{5} = 1 = \text{100% Efficiency}\\
  \text{Speedup}_{10} & = \frac{15}{2} = 7.5\\
  \text{Efficiency}_{10} & = \frac{7.5}{10} = 0.75 = \text{75% Efficiency}
\end{align}
$$
  </div>
</div>

## Adding RAM constraints



As we talked about previously in [Single Core Computing]({{ site.baseurl}}/pedagogic_modules/single_core_computing), 
when a core is running a task, the task may have a sizeable amount of data that needs to be loaded into RAM. What can 
cause additional issues on a multi-core machine is that all of the cores are sharing the same amount of RAM. So what 
happens when there are cores free and tasks that need to run, but there is not sufficient RAM? Unfortunately, the answer
is that they must wait to run, the extra cores will sit idle, and parellel efficiency falls. 


#### Practice Questions 

- A machine runs 5 tasks that each take a core 1 second to complete. The current single core processor takes 5 seconds. 
The processor is then updated to 5 cores, each identical in processing power to the single core they replaced. If the 
machine has 8GB of RAM and each task takes 2GB, what is the efficiency of upgrading from 1 core to 5. 


<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   Previously the RAM constraint was not a problem as the single core completed the tasks sequentially, never needing 
   more than 2GB of RAM. Now that there are 5 cores, we require 10GB of RAM to run all the tasks concurrently, but only
   8GB is available.

$$
\begin{align}
  \text{Speedup}_5 & = \frac{5}{2} = 2.5\\
  \text{Efficiency}_5 & = \frac{2.5}{5} = 1 = \text{50% Efficiency}
\end{align}
$$
  </div>
</div>

- If you are working with a 4-core machine that will be running a variety of tasks that use 2, 4, 6 or 8GB of RAM 
during execution, what is the minimum RAM that the machine needs to prevent idle time due to lack of memory?


<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   As there are four cores and they could each potentially be processing a task that requires 8GB of RAM, we should 
   ensure that there is at least 32GB of RAM available. 

  </div>
</div>


<!--  template    
        
<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   When using only a single core, the 15 tasks will take 15 seconds to complete. When increasing the number of cores to 
      5,
   the same tasks can now be done in 3 seconds. When increasing the number of cores to 10, the tasks take 2 seconds. What 
   is important to notice is that for the last second, 5 of the cores in the 10 core processor will be idle. Since our main
   concern for this exercise is efficiency rather than raw speed, we would go with the 5 core processor.

$$
\begin{align}
  \text{Speedup}_5 & = \frac{15}{3} = 5\\
  \text{Efficiency}_5 & = \frac{5}{5} = 1 = \text{100% Efficiency}\\
  \text{Speedup}_{10} & = \frac{15}{2} = 7.5\\
  \text{Efficiency}_{10} & = \frac{7.5}{10} = 0.75 = \text{75% Efficiency}
\end{align}
$$
  </div>
</div>
        
-->




<!--
When looking at sequential computing efficiency is a simple concept, the processor is either working a task or the 
program has completed. When looking at multi-core computing however, the notion of efficiency becomes important as part 
of the processor (some cores) may be working while others sit idle, and doubling the number of cores usually will not 
double the speed.  
-->

## Exercises

For the remainder of this activity, we will be using the visualization tool. In the terminal, run the following
commands:
1. run `docker pull wrenchproject/wrench-pedagogic-modules:activity-visualization`
2. then run `docker container run -p 3000:3000 -d  wrenchproject/wrench-pedagogic-modules:activity-visualization`
3. open a browser and go to [localhost:3000/](localhost:3000/)
4. sign in using your `<UH Username>@hawaii.edu` Google Account
5. select `Multi-core Machines`

These exercises accompany the simulator "Multi-core Machines". You will need to do calculations similar to those done in
the sequential computing section, but now it will be the number of cores and tasks that vary, rather than the flops. 

##### For These Questions, Tasks require 0 RAM. 

**[q1]** What is the speedup that you will observe for running ten tasks on one core versus ten tasks on two cores? What
is the efficiency?

**[q2]** What is the speedup that you will observe for running ten tasks on one core versus ten tasks on three cores? 
What is the efficiency?

##### For These Questions, RAM Will Vary.

**[q3]** The host we are using has 32GB of ram, how long does it take to run two tasks on two cores if they each require
16GB of RAM? What if they require 20GB of RAM each?
        
**[q4]** Figure out what happens for the scenario described in Question 1, but this time the tasks require 
16GB of RAM. What are the speedup and efficiency now?

