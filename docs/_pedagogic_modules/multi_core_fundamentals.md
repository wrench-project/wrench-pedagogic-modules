---
layout: page
title: 'B. Multi-Core Machines'
order: 200
usemathjax: true
---

This section is designed to introduce you to the basics of using a machine with a multi-core processor.


#### Learning Objectives:

  - Understand the advantages of using a multi-core machine

  - Encounter concepts of speedup and efficiency
  
  - Understand the impact of RAM constraints for utilizing multiple cores.

----



# Multi-Core Machines

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
hour, now you can complete the same four tasks in two hours. Most real world applications do not work in this fashion, 
so we have *Speedup* and *Efficiency* to evaluate any changes made to processing.

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
of the processor (some cores) may be working while others sit idle, and doubling the number of cores usually will not 
double the speed.  

The formula for efficiency is below: 

$$
\begin{align}
\text{Efficiency} & = \frac{\text{Speedup}}{\text{Number of Cores}}
\end{align}     
$$

If the Speedup going from a single core to three cores is two, then the efficiency can be found by:

$$
\begin{align}
\text{Efficiency} & = \frac{\text{2}}{\text{3}} & = \text{0.667}
\end{align}     
$$

This means that because using three cores is not three times as fast, we have imperfect efficiency. 

## Exercises

These exercises accompany the simulator "Multi-core Machines". You will need to do calculations similar to those done in
the sequential computing section, but now it will be the number of cores and tasks that vary, rather than the flops. 

**[q1]** What is the speedup that you will observe for running ten tasks on one core versus ten tasks on two cores? What
is the efficiency?

**[q2]** What is the speedup that you will observe for running ten tasks on one core versus ten tasks on three cores? 
What is the efficiency?

**[q3]** RAM QUESTION 1

**[q4]** RAM QUESTION 2

