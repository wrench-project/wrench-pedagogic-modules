# wrench is the name of the 'wrench' docker image which was created with the base dockerfile in the root directory
FROM wrenchproject/wrench-pedagogic-modules:wrench-1.0 

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout gh-pages for "published" version, build
# right now it is private, going to copy files from workdir for now

RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
    && cd wrench-pedagogic-modules/activity_1_getting_started \
    && git checkout master\
    && cmake . \
    && make
    
RUN chown -R wrench ./wrench-pedagogic-modules

USER wrench
WORKDIR /home/wrench/wrench-pedagogic-modules/activity_1_getting_started

CMD ./activity_simulator platform_files/platform.xml workflow_files/workflow.dax 
