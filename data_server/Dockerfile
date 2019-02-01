FROM node:10.15-stretch

USER root

RUN useradd -ms /bin/bash  data_server

USER data_server

WORKDIR /home/data_server/

# pull down data server
RUN git clone https://github.com/wrench-project/wrench-pedagogic-modules.git \
    && cd wrench-pedagogic-modules \
    && git checkout ics332_s19

WORKDIR /home/data_server/wrench-pedagogic-modules/data_server

# create a directory where json data will be stored
RUN mkdir data

# keys that are not versioned are placed into this image
COPY keys.js /home/data_server/wrench-pedagogic-modules/data_server/keys.js

# set this path to be a volume so that data can persist beyond the container
VOLUME /home/data_server/wrench-pedagogic-modules/data_server/data

EXPOSE 3001

USER data_server
CMD ["node", "app.js"]
