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

This activity does not introduce any new concepts, but instead incorporates
all concepts acquired in the previous activities.  It presents you with a
case study that will require you to analyze a given workload on a given
cyber-infrastructure. Then you will need to come up with an argument as to
how this workload can be executed faster, given certain constraints. You
will be guided through the decision making process so that you can build up
your argument.

## Case Study

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_3/workflow.svg">Workflow</object>

You are a software engineer at a large corporation that routinely executes
workflows for many customer applications. You have been tasked with
ensuring that a particularly important workflow (Figure 1), which is
executed repeatedly on different input files, runs as efficiently as
possible on your corporation's cyber infrastructure (Figure 2).  Your
customers produce the input files, which are always the same size, at the
remote storage service.  The workflow's single output file must be stored,
once it's been created, at that same storage service. All computation takes
place on your corporation's compute service (CS).

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_3/cyber_infrastructure.svg">Platform</object>

The Workflow Management System (WMS) that your corporation uses for this
workflow submits tasks to the CS with the following instructions regarding
file operations (Figure 3): read the initial input files from the remote
storage service, write the final output file to the remote storage service,
read and write all other files using the CS's scratch space.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_3/wms.svg">WMS Secenario</object>

### The Problem

Up until now, your customers have been producing input files at roughly the
same rate as you are able to execute the workflow.  However, they have
recently upgraded their process and now produce input files at a faster
rate. The workflow execution on the cyberinfrastructure can no longer keep
up and has become a performance, and revenue, bottleneck!

You immediately get in touch with your boss about this, and come to the
agreement that **$1500** will budgeted for to you to upgrade the
cyberinfrastructure so that the workflow execution can be accelerated as
much as possible (which hopefully will be fast enough).

Your boss does not care if the total amount spent is less than the allotted
$1500, but before making the purchase you must make a valid case as to why
the upgrade(s) you choose is/are warranted.


# Activity

## Step 1: Evaluate All the Options

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/activity_3/upgrades.svg">Upgrades</object>

Your cyberinfrastructure provider says they can offer you several hardware
upgrades for various costs, as shown in Figure 4.
Given your budget, you thus have 3 options:

| Option | Cores |  RAM  | Link Bandwidth |  Cost  |
|:------:|:-----:|:-----:|:--------------:|:------:|
|    1   |   2   | 32 GB |   1,000 MBps   | $1,500 |
|    2   |   4   | 16 GB |   1,000 MBps   | $1,400 |
|    3   |   4   | 32 GB |    100 MBps    |  $700  |


**Answer these questions**

- [q1] Without an upgrade, about how long do you estimate the workflow execution to take?
        Write a simple formula that shows your work.
- [q2] Going with *Option 1*, about how long do you estimate the workflow execution to take?
        Write simple formula that shows your work.
- [q3] Going with *Option 2*, about how long do you estimate the workflow execution to take?
        Write simple formula that shows your work.
- [q4] Going with *Option 3*, about how long do you estimate the workflow execution to take?
        Write simple formula that shows your work.

## Step 2: Select the Best Upgrade

**Answer these questions**

- [q5] Which option gives you the fastest workflow execution time while keeping utilization as high as possible? Your answer
       is of course based on your answers in the previous step. But also give an intuitive reason why the best option is
       indeed the best.
- [q6] What is the CS's utilization with the best option?
- [q7] How does it compare with the utilization of the current non-upgraded cyberinfrastructure?
- [q8] Write several sentences (not more than 5) to convince your boss that you have found the best hardware upgrade. Use your findings from the previous step to strengthen your claim.

## Step 3: Considering Other Scenarios

There is some notion that data will change in the future, with different file sizes. Although this has not happened
yet, your boss asks you for some information about how different file sizes may change your current decisions.

**Answer these questions**

- [q9] Assuming the input file size is now 7 GB (all three files), which option would be the *best option*?
- [q10] At what input file size would the *best option*  (i.e., your answer for q5) no longer be the best option?

# Conclusion

In this activity, we have put to use some of the concepts you have learned thus far. By having
a good understanding of your workload and the platform on which it runs, you were able to make
a calculated decision as to what hardware upgrades would benefit you the most. First, you identified
the current workflow execution time as a baseline performance metric. Second, you enumerated the possible
upgrade options and, using back-of-the-envelope calculations, estimated how they compared with the baseline.
Finally, you used that data to make a sound decision as to which hardware upgrade would best meet the
requirements laid out by your boss. As a result, the money was put to good use and workflow execution
performance is improved, for now...

Based on your answers from Step #3, it should be clear that the *best option*   
is not always the *best*. When file sizes vary, one option might be better than the other.
Purchasing a certain hardware upgrade may be the right
way to approach a performance related issue in some cases but not all.
In the next activity, "Activity 4: Some Name To Do With Varying File Sizes", we explore
the effects of varying file sizes on workflow execution performance.  
