---
layout: default
order: 0
---
## Welcome to the [WRENCH](http://wrench-project.org)-Pedagogic-Modules,

The goal of these pedagogic modules is for students to achieve learning objectives
in the area of distributed computing, in the context of scientific workflows, via
hands-on experiments. This is achieved through a set of “activities” that explore,
at first elementary and later sophisticated, concepts and techniques. All these
activities entail simulating workflow executions using the
[WRENCH](http://wrench-project.org) simulation framework.

Each activity can be viewed by clicking on the menu button located at the top
left hand corner of the screen. Before proceeding to the first section,
[What is this about?]({{ site.baseurl }}/activities/what_is_this_about), ensure
that you are using one of the web browsers specified below and have [Docker](https://docker.com)
OR [Vagrant](https://www.vagrantup.com/) installed on your machine.

### Software Requirements
----
- latest version of either of the following web browsers:
    - [Google Chrome](https://www.google.com/chrome/)
    - [Firefox](https://www.mozilla.org/en-US/firefox/new/)
    - [Safari](https://www.apple.com/safari/)

- [Docker](https://docker.com)
    - installation instructions:
      - [Linux](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
      - [macOS](https://docs.docker.com/docker-for-mac/install/)
      - [Windows 10 Pro](https://docs.docker.com/docker-for-windows/install/)

- [Vagrant](https://www.vagrantup.com/)
    - installation instructions:
      1. Install [VirtualBox](https://www.virtualbox.org/).
      2. Install [Vagrant binary](https://www.vagrantup.com/downloads.html).
      3. Download this [Vagrantfile]({{ site.baseurl }}/public/Vagrantfile).
      4. In a terminal, navigate to the same directory where the `Vagrantfile`
        from step 3 is located and run the command `vagrant up`. This may take
        a few minutes.
      5. Whenever an activity instructs you to run a `docker` command, you will
      need run the command from within the Vagrant virtual machine. To enter
      the Vagrant virtual machine, navigate to the same directory where the
      `Vagrantfile` is located and run the command `vagrant ssh`.
