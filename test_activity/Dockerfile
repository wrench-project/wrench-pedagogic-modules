FROM wrench 

USER root

WORKDIR /home/wrench/

# pull pedagogic-modules, go to desired activity, checkout gh-pages for "published" version, build
RUN git clone <wrench-pedagogic-modules> \
    && cd <acitivity_N> \
    && git checkout gh-pages \
    && cmake . \
    && make
    
RUN chown -R wrench ./<wrench-pedagogic-modules>

USER wrench
WORKDIR /home/wrench/wrench-pedagogic-modules/activity_N

CMD ./simulation-executable platform_files/platform.xml workflow_files/workflow.dax 
