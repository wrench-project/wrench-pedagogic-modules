# wrench is the name of the 'wrench' docker image which was created with the base dockerfile in the root directory
FROM wrenchproject/wrench-pedagogic-modules:wrench-1.0

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout master for "published" version, build
RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
    && cd wrench-pedagogic-modules/activity_0_network_primer \
    && cmake . \
    && make

RUN chown -R wrench ./wrench-pedagogic-modules

USER wrench
WORKDIR /home/wrench/wrench-pedagogic-modules/activity_0_network_primer

ENTRYPOINT ["./activity_0_simulator", "--log=root.thresh:critical"]
