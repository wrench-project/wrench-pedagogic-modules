FROM wrenchproject/wrench-build:ubuntu-xenial-gcc5

#################################################
# INSTALL WRENCH
#################################################

USER root
WORKDIR /tmp

# clone WRENCH, checkout version 1.2, build, and install
RUN git clone https://github.com/wrench-project/wrench \
    && cd wrench \
    && git checkout fa11d197\
    && cmake . \
    && make \
    && sudo make install

# remove installation folder
RUN rm -rf wrench

#################################################
# WRENCH's user
#################################################

USER wrench
WORKDIR /home/wrench

# set user's environment variable
ENV CXX="g++-5" CC="gcc-5"
ENV LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
