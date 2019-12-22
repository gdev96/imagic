#!/bin/bash

# compile server

cd ../server
mkdir -p build
cd build
cmake ..
make

# compile load balancer

cd ../../load_balancer
mkdir -p build
cd build
cmake ..
make

# start server

cd ../../server/build
./server &

# wait for server to be ready

sleep 2

# start load balancer

cd ../../load_balancer/build
./load_balancer &

# wait for load balancer to be ready

sleep 2

# execute client

cd ../../client
python3 main.py

