# wrench is the name of the 'wrench' docker image which was created with the base dockerfile in the root directory
FROM wrench 

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout gh-pages for "published" version, build
# right now it is private, going to copy files from workdir for now

#RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
#    && cd test_activity \
#    && git checkout 536916f \
#    && cmake . \
#    && make
    
#RUN chown -R wrench ./wrench-pedagogic-modules

# TESTING ################################
RUN mkdir test_activity
COPY ./ test_activity/

RUN cd test_activity && cmake . && make
##########################################

USER wrench
WORKDIR /home/wrench/test_activity

CMD ./wrench-simple-wms-cloud platform_files/cloud_hosts.xml workflow_files/genome.dax 
