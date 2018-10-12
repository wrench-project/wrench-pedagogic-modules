# wrench is the name of the 'wrench' docker image which was created with the base dockerfile in the root directory
FROM wrenchproject/wrench-pedagogic-modules:wrench-1.0

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout gh-pages for "published" version, build
RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
    && git checkout master 

RUN chown -R wrench ./wrench-pedagogic-modules

# build binaries for each activity
WORKDIR /home/wrench/wrench-pedagogic-modules

RUN cd activity_1_getting_started \
    && ./build.sh

RUN cd activity_2_parallelism \
    && ./build.sh

# install Node 10.x for the visualization
RUN sudo apt install curl \
    && curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash - \
    && sudo apt-get install -y nodejs

USER wrench

EXPOSE 3000

WORKDIR /home/wrench/wrench-pedagogic-modules/visualization
CMD ["node", "app.js"]
