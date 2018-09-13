cmake -DCMAKE_CXX_FLAGS="-DDEBUG" . && make clean && make && mv ./activity_2_simulator ./activity_2_simulator_debug \
    && cmake -DCMAKE_CXX_FLAGS="-UDEBUG" . && make clean && make
