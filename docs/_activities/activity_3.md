---
layout: page
title: 'Activity 3: The Right Tool for the Job'
order: 4
---

1. [Learning objectives](#learning-objectives)
2. [Overview](#overview)
2. [Case Study](#case-study)
3. [Activity](#activity)
3. [Conclusion](#conclusion)

# Learning Objectives
- Be able to make calculated system design decisions;
- Be able to apply the concepts explored in the previous activities;

# Overview

This activity focuses on incorporating the concepts we have touched upon in the
previous sections. Here, we present you with a case study that will require
you to analyze a given workload on a given cyber-infrastructure. Then you will
need to come up with an argument as to how this workload can be executed faster,
given certain constraints. We will guide you through the decision making process
by providing questions you can answer to help build up your argument.

## Case Study

You are a software engineer at a
large research institution in charge of developing workflow management systems (WMS). Your
current work involves repeatedly executing a single workflow (Figure 1) on the cyber
infrastructure (Figure 2) provided to you by the institution. Your research collaborators produce the
initial input files to this workflow and posit them into the remote storage service.
These input files are always the same size.
Additionally, your collaborators are interested in the results yielded by the workflow (the single output file).
It is your job to access the remote storage service to obtain the input files for processing.
Then, when the workflow is complete, you must share your results (the single output file) by sending
the output of the workflow back to the remote storage service.

**Figure 1 the workflow**, **Figure 2 the platform**

The WMS you have developed specifically for this workflow submits tasks to the
compute service (CS) with the following instructions regarding file operations (Figure 3):
read the initial input files from the remote storage service, write the final output
file to the remote storage service, read and write all other files using the CS’s scratch space.

**Figure 3 the wms scenario**

### The Problem

Up until now, your collaborators have been producing the workflow's input files at around the
same rate as you are able to execute the workflow, and so the work is pipelined such that
your collaborators produce input file set "n+1" while you process the input file set "n" by executing
the workflow with your WMS.


However, they have just upgraded their system and now produce input
files at a much faster rate. What does this mean? Your role in the research endeavor has now become the
bottleneck and we need to mitigate this issue!


You immediately get in touch with your boss come to the agreement that **$1500** will
budgeted for to you to fulfill the following requirements, with requirement #1 being of
the highest priority:
1. **the workflow** will be **executed as fast as possible**
2. **utilization** will be **as high as possible**


# Activity

**Figure 4 the hardware upgrades**

The goal here is to purchase the hardware upgrade(s) (Figure 4) that will **minimize the
amount of time it takes to execute the workflow**. Your boss does **not** care
if the total amount spent is less than the allotted $1500. She is only concerned
that you make the purchase that speeds up the workflow execution the most. Furthermore,
before making the purchase, you must make a valid case as to why the upgrade(s) you
choose is/are warranted.

**Figure 4 the possible hardware upgrades**

## Step 1: Enumerate Your Options

Since your boss does not care how much of the $1500 you spend, we opt to spend
the maximum amount. That gives us three options to choose from.

| Option | Cores |  Ram  | Link Bandwidth |  Cost  |
|:------:|:-----:|:-----:|:--------------:|:------:|
|    1   |   2   | 32 GB |   1,000 MBps   | $1,500 |
|    2   |   4   | 16 GB |   1,000 MBps   | $1,400 |
|    3   |   4   | 32 GB |    100 MBps    |  $700  |

## Step 2: Evaluate Your Options

**Answer these questions**

- [q1] Without an upgrade, about how long can we estimate the workflow execution to take?
        Write simple formula.
- [q2] Going with *Option 1*, about how long can we estimate the workflow execution to take?
        Write simple formula.
- [q3] What would the utilization of the CS be if you went with *Option 1*? Show your work.
- [q4] Going with *Option 2*, about how long can we estimate the workflow execution to take?
        Write simple formula.
- [q5] What would be the utilization of the CS be if you went with *Option 2*? Show your work.        
- [q6] Going with *Option 3*, about how long can we estimate the workflow execution to take?
        Write simple formula.
- [q7] What would the utilization of the CS be if you went with *Option 3*? Show your work.

## Step 3: Select the Best Upgrade

**Answer these questions**

- [q8] Which option gives you the fastest workflow execution time while keeping utilization as high as possible?
- [q9] Write several sentences convincing your boss that you have found the best hardware upgrade. Use your findings
  from the previous step to strengthen your claim.

## Step 4: Considering Other Scenarios

**Answer these questions**

- [q10] At what input file size would *Option 1* result in a similar workflow execution time as the *best option*?
- [q11] Assuming the input file size is now 7 GB (all three files), which option would be the *best option*?

# Conclusion

In this activity, we have put to use some of the concepts you have learned thus far. By having
a good understanding of your workload and the platform on which it runs, you were able to make
a calculated decision as to what hardware upgrades would benefit you the most. First, you identified
the current workflow execution time as a baseline performance metric. Second, you enumerated the possible
upgrade options and, using back-of-the-envelope calculations, estimated how they compared with the baseline.
Finally, you used that data to make a sound decision as to which hardware upgrade would best meet the
requirements laid out by your boss. As a result, the money was put to good use and workflow execution
performance is improved, for now...

Based on your answers from Step #4, it should be clear that the *best option*   
is not always the *best*. When file sizes vary, one option might be better than the other.
Purchasing a certain hardware upgrade may be the right
way to approach a performance related issue in some cases but not all.
In the next activity, "Activity 4: Some Name To Do With Varying File Sizes", we explore
the effects of varying file sizes on workflow execution performance.  
