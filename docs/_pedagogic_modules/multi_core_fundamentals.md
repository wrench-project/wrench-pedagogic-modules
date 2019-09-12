---
layout: page
title: 'B. Multicore Computing Fundamentals'
order: 200
usemathjax: true
---

under construction

#### Learning Objectives:

  - Understand the advantages of using a multi-core machine

  - Encounter concepts of speedup and efficiency

----



# Multi-Core Execution

1. [Basics](#basics)
2. [Benefits](#benefits)
    * [Speedup](#speedup)
    * [Efficiency](#efficiency)
3. [Exercises](#exercises)


## Basics

Multi-core processors have become ubiquitous in modern computing due to well-documented issues with further increasing
 single core performance. When executing multiple programs or tasks it will be necessary to take into account the number
 of cores available to you and the capability of your tasks to make use of those cores. Each core is a processor capable
 of running its own tasks concurrently with those run by the other cores. 
 
For our purposes in this lesson, we will be simplifying some aspects of using multiple cores. Throughout the lesson, 
terms such as *tasks* and *programs* will be used interchangeably to refer to discrete processes with their own unshared
 memory allocation. We will not be exploring multithreading just yet, where shared memory can come into play.
 
One other simplification that will be found is the use of application scheduling to run tasks, rather than scheduling 
done via the operating system. Within this lesson it will be assumed that a task that begins running on a core will run 
until completion on that same core. 



## Benefits

The benefits of using multiple cores become most apparent when each core is equivalent in power to a processor that has 
a single core. For example, if you have tasks that the single-core processor can complete in one hour, it will take four
hours to complete four tasks. If you have a dual-core processor where each core is capable of completing a task in an 
hour, now you can complete the same four tasks in two hours. 

### Speedup

Speedup is a metric used to quantify how much improvement has taken place. It is calculated by dividing the previous 
metric by the new metric. This can be the time taken for processing, or cycles per instruction (CPI). Take a look at the
formula below:

$$
\begin{align}
\text{Speedup} & = \frac{\text{Compute Time}_{old}}{\text{Compute Time}_{new}}
\end{align}     
$$


### Efficiency

When looking at sequential computing efficiency is a simple concept, the processor is either working a task or the 
program has completed. When looking at multi-core computing however, the notion of efficiency becomes important as part 
of the processor (some cores) may be working while others sit idle. 

The formula for efficiency is below: 

TODO: formula for efficiency

## Exercises

**[q1]**

**[q2]**

**[q3]**

**[q4]**

