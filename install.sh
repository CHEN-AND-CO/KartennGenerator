#!/bin/sh

set -e

dependencies(){
    echo 'Installs C++ dependencies and tools'
    sudo apt -y install gcc g++ cmake mapnik* libpqxx-dev libmapnik-dev nlohmann-json3-dev
}

build(){
    echo 'Configuring...'
    mkdir -p build
    cd build
    cmake ..
    echo 'Building...'
    make -j
}

dependencies
build