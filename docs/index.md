---
layout: default
order: 0
---
## Welcome to the [WRENCH](http://wrench-project.org)-Pedagogic-Modules,

On this site students can learn important concepts
 and techniques in the areas of parallel, distributed, and high-performance computing.
This is achieved through *modules* that can provide hands-on leaning experiences via
*simulators* developed with the [WRENCH](http://wrench-project.org/)
framework. Proceed by going through the module sequence in the navigation bar on the
left-hand side. 


----

### Software Requirements

Make sure you are using a supported Web browser, and have [Docker](https://docker.com)
OR [Vagrant](https://www.vagrantup.com/) installed on your machine:

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
