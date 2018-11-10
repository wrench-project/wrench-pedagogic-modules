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

This activity focuses on incorporating the concepts we've touch upon in the
previous sections. Here, we present you with a case study that will require
you to analyze a given workload on a given cyber-infrastructure. Then you will
need to synthesize an argument as to how this workload can be executed faster,
given certain constraints. We will guide you through the decision making process
by providing questions you can answer to help build up your argument.

## Case Study

You are a software engineer at a  <!--the WRENCH Institute for Research and Education (WIRE)... -->
large research institution in charge of developing workflow management systems (WMS). Your
current work involves repeatedly executing a single workflow (Figure 1) on the cyber
infrastructure (Figure 2) provided to you by the institution. Your research collaborators produce the
initial input files to this workflow and posit them into the remote storage service. Additionally,
your collaborators are interested in the results yielded by the workflow (the single output file).
It is your job to access the remote storage service to obtain the input files for processing.
Then, when the workflow is complete, you must impart your results (the single output file) by sending
it back to the remote storage service.

**Figure 1 the workflow**, **Figure 2 the platform**

The WMS you have developed specifically for this workflow submits tasks to the
compute service (CS) with the following instructions regarding file operations (Figure 3):
read the initial input files from the remote storage service, write the final output
file to the remote storage service, read and write all other files using the CS’s scratch space.

**Figure 3 the wms scenario**

### The Problem

Up until now, your collaborators have been producing the input files at around the
same rate as you are able to execute the workflow. An efficient pipeline has been established such
that
your collaborators produce input file set "n+1" while you process the input file set "n" by executing
the workflow with your WMS. However, they have just upgraded their system and now produce input
files at a much faster rate. What does this mean? Your role in the research has now become the
bottleneck and we need to do whatever possible to mitigate this issue!


You immediately get in touch with your boss and arrange for $1500 to be set aside
for you to purchase hardware upgrades...

# Activity

The goal here is to purchase the hardware upgrade(s) (Figure 4) that will **minimize the
amount of time it takes to execute the workflow**. Your boss does **not** care
if the total amount spent is less than the allotted $1500. She is only concerned
that you make the purchase that speeds up the workflow execution the most. Furthermore,
before making the purchase, you must make a valid case as to why the upgrade(s) you
choose is/are warranted.

**Figure 4 the possible hardware upgrades**


# Conclusion
