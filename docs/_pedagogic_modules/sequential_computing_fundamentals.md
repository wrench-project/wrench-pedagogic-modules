---
layout: page
title: 'A. Sequential Computing Fundamentals'
order: 10
usemathjax: true
---

#### Learning objectives:

In this first module you acquire a basic, but
fundamental, understanding of the performance of a computer that executes a
program. We consider here the execution of a sequential (i.e., non
multi-threaded) program that runs on a single core of some microprocessor
in a computer. The learning objectives are:

  - Understand the concepts of *work* and of *compute speed*;

  - Be familiar with Flop as a measure of work and with Flop/sec
    as a measure of compute speed;

  - Understand the simple relationship between program execution time, 
    work, and compute speed.

---


### Measures of Work and Compute Speed

Consider a non-interactive program that has some amount of *work* to do and
then terminates (e.g., a program that encrypts a file, a program that
applies some cool filter to a set of images). The simplest model of
performance when executing the program on a computer is to assume that the
computer delivers constant *compute speed*, which is measured in the quantity of
work performed per time unit. For instance, a program with 50 units of work would
run on a computer with speed 2 units of work per second in 100 seconds.
 This last number is called the program's
*execution time*. Generalizing this example, for a given amount of work to
perform, there is a linear relationship between the program's execution
time and the speed of the computer on which it is executed:

$$
\begin{align}
\text{execution time} & = \frac{\text{work}}{\text{compute speed}}
\end{align}     
$$

There are many options for choosing an appropriate ways to quantify work.
One possibility is to use a measure that is specific to what the program
does. For instance, if the program renders movie frames, a good measure of
work would be the number of frames to render.  One would then want to measure a
computer's speed in terms of the number of frames that can be rendered per second. 
Another possibility is to use a more generic measure, for instance, the
number of instructions.  The work of a program would then be measured by
its number of instructions (e.g., the number of assembly instructions the
program performs) and the speed of a computer would be in number of
instructions per second. This approach is known to have problems, as
instructions are different and especially across different kinds of
computers. Therefore, a processor that delivers fewer instructions per
seconds than another could actually be preferable for running some program.

It turns out that he question of modeling/predicting how fast a particular
program will run on a particular computer is very challenge (the only way
to be sure if to actually run the program on all computers of interest and
compare). 


### Flop and Flop/sec

In these pedagogic modules, unless specified otherwise, we use a simple
measure of work: the number of floating-point operations, or **Flop**, that
the program performs.  We thus measure the speed of a computer in Flop/sec.
This measure is typical in the fields of high-performance scientific computing.
But like any generic
measure it is imperfect (e.g., programs do non-floating-point computations,
floating-point operations are not all the same).  
All the concepts we learn are actually agnostic to the way in which we measure work,
so we just pick Flop to be consistent throughout. 


For example, say a program that performs 100 TFlop ("100 TeraFlop") is executed 
on a computer with speed 35 GFlop/sec ("35 GigaFlop per second"). The
program's execution time would then be:

$$
\begin{align}
 \text{execution time} & = \frac{100 \times 10^{12}\; \text{Flop}}{25 \times 10^{9}\; \text{Flop/sec}}\\
   & \simeq 2,857.14\; \text{sec}
\end{align}
$$  

If a program that performs 12 GFlop runs in 5 seconds on a computer, then the speed of this computer in MFlop/sec ("MegaFlop per second") is:

$$
\begin{align}
 \text{speed} & = \frac{12 \times 10^{9}\; \text{Flop}}{5 \; \text{sec}} \times \frac{1}{10^{6}}\\
       & = 2,400 \; \text{MFlop/sec}
\end{align}
$$  


---

#### Practice Questions

To make sure the above is crystal clear, try to answer the following questions:

**[q1]** You have to run a program that will perform 4000 GFlop, and your
computer computes at speed 30 TFlop/sec. How long will the program run for?

<div class="ui accordion fluid">
  <div class="title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   $$ \frac{4 \;\text{TFlop}}{30\; \text{TFlop/sec}} \simeq 0.13\; \text{sec}   $$
  </div>
</div>

<p> </p>

**[q2]** A program just ran in 1640 sec on a computer with speed 2 TFlop/sec, 
how many MFlop does the program perform?
<div class="ui accordion fluid">
  <div class=" title">
    <i class="dropdown icon"></i>
    (click to see answer)
  </div>
  <div markdown="1" class="ui segment content">
   $$ \frac{2000000\; \text{MFlop/sec}}{1640\; \text{sec}} \simeq 1219.5 \;\text{MFlop} $$
  </div>
</div>


