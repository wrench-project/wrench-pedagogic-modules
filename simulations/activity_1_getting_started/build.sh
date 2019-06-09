cmake -DCMAKE_CXX_FLAGS="-DREMOTE_STORAGE" . && make clean && make && mv ./activity_simulator ./simulator_remote_storage \
    && cmake -DCMAKE_CXX_FLAGS="-DLOCAL_STORAGE" . && make clean && make && mv ./activity_simulator ./simulator_local_storage
    
