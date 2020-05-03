#!/bin/sh

set -e

function dependencies(){
    echo 'Installs C++ dependencies and tools'
    sudo apt -y install gcc g++ cmake mapnik mapnik-doc mapnik-reference mapnik-utils libpqxx-dev libmapnik-dev nlohmann-json3-dev
}

function build(){
    echo 'Configuring...'
    mkdir -p build
    cd build
    cmake ..
    echo 'Building...'
    make -j
}

dependencies()
build()