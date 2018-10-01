# wrench is the name of the 'wrench' docker image which was created with the base dockerfile in the root directory
FROM wrenchproject/wrench-pedagogic-modules:wrench-1.0

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout gh-pages for "published" version, build
RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
    && cd wrench-pedagogic-modules/activity_1_getting_started \
    && git checkout master\
    && cmake -DCMAKE_CXX_FLAGS="-DREMOTE_STORAGE" . && make clean && make && mv ./activity_simulator ./simulator_remote_storage \
    && cmake -DCMAKE_CXX_FLAGS="-DLOCAL_STORAGE" . && make clean && make && mv ./activity_simulator ./simulator_local_storage

RUN chown -R wrench ./wrench-pedagogic-modules

# install Node 10.x for the visualization
RUN sudo apt install curl \
    && curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash - \
    && sudo apt-get install -y nodejs

USER wrench

EXPOSE 3000

WORKDIR /home/wrench/wrench-pedagogic-modules/activity_1_getting_started/visualization
CMD ["node", "app.js"]
