cmake -DCMAKE_CXX_FLAGS="-DREMOTE_STORAGE" . && make clean && make && mv ./networking_fundamentals_simulator ./networking_fundamentals_simulator \
    && cmake -DCMAKE_CXX_FLAGS="-DLOCAL_STORAGE" . && make clean && make && mv ./networking_fundamentals_simulator ./networking_fundamentals_simulator
