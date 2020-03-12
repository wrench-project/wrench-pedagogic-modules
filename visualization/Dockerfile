# wrench is the name of the 'wrench' docker image which was created with the base dockerfile in wrench-pedagogic-modules/simulations/Dockerfile
FROM wrenchproject/wrench-pedagogic-modules:wrench-1.5dev

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout gh-pages for "published" version, build
RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
    && cd wrench-pedagogic-modules \
    && git checkout master

COPY keys.js /home/wrench/wrench-pedagogic-modules/visualization/keys.js

RUN chown -R wrench ./wrench-pedagogic-modules

# build binaries for each activity
WORKDIR /home/wrench/wrench-pedagogic-modules/simulations

RUN cd networking_fundamentals \
    && ./build.sh

RUN cd multi_core_computing \
    && ./build.sh

RUN cd client_server \
    && ./build.sh

RUN cd workflow_execution_fundamentals \
    && ./build.sh

RUN cd workflow_execution_data_locality \
    && ./build.sh

RUN cd workflow_execution_parallelism \
    && ./build.sh




# install Node 10.x for the visualization
RUN sudo apt update \
    && sudo apt install -y curl \
    && curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash - \
    && sudo apt-get install -y nodejs

USER wrench

EXPOSE 3000

WORKDIR /home/wrench/wrench-pedagogic-modules/visualization
CMD ["node", "app.js"]
