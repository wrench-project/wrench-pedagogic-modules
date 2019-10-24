---
layout: page
title: 'Input/Output Effects'
order: 6
usemathjax: true
---



### Learning Objectives

	- Understand the impact of IO operations on computing
	- Understand basics of optimizing computation around IO operations


### Basic Concepts

In computing, the processor will not be able to complete a program start to finish in a vacuum. Programs will often 
need to wait on input or output, referred to as *IO operations*. A couple very common IO operations are reading from 
or writing to disk. As the disk is much slower than the CPU, even small reads or writes will represent a large chunk 
of time that the CPU is sitting idle. 

When it comes to IO operations, not every program is created equal. Some programs will need more IO time than others 
and can even be categorized in this way. If a program requires a lot of IO time, it can be referred to as IO intensive. 
If a program has little need for IO compared to its CPU usage, it is referred to as CPU intensive. 

As mentioned above, reading from and writing to the disk are slow operations compared to the CPU. What also needs to be 
taken into account is that there is a difference between read and write speed as well. Reading from the disk is 
typically faster than writing to the disk by a noticeable margin. On top of this difference, different kinds of memory 
have various speeds as well. To look at the difference between the two main types of main storage, 
Hard Disk Drives (HDD) and Solid State Drives (SSD), the table below contains general figures for consumer-grade 
hardware. 


|     | Read     | Write    |
|-----|----------|----------|
| HDD | 210 MBps | 200 MBps |
| SSD | 550 MBps | 520 MBps |


### How Computation With IO Looks

Let us examine a program that needs to read from disk prior to computation, and then write back to disk afterwards. This
could be applying the oil painting filter to a photograph. Each task will need to load the photograph from disk, 
apply the filter, and then save the new image back to disk. 

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/IO_effects/IO_figure_1.svg">Cyberinfrastructure</object>

As can be seen in the figure above, at any given time there is only either CPU computation or IO operations ongoing. 
This means that the CPU is idle for the entire time that is spent doing IO. This is inefficiency that can be dealt 
with because IO and CPU actions are capable of being done independently. While the CPU is working on the first image, 
the second image can be loaded into memory from disk. This will allow the CPU to immediately begin task two upon 
completion of the first task, and while it is working on task two, the first task is being written to disk.

<object class="figure" type="image/svg+xml" data="{{ site.baseurl }}/public/img/IO_effects/IO_figure_2.svg">Cyberinfrastructure</object>

As you can see from the above representation, not only does the overall execution time drop significantly, but the CPU 
is also working for a greater percentage of the time. If there were additional tasks added, the idle time would still 
only occur at the very start and very end of the process. 

This is a very neat situation however, and it would not always be realistic in reality. Take the cases below for 
example:

[Inserting SVG here]

In the first case, we now have read and write times that together require a longer amount of time than it takes the CPU
to process one task. This will make weaving CPU processing and IO together not quite as pretty. 

[Inserting SVG here]

In the second case, now the 





### Practice Questions




### Questions