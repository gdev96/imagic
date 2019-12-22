#!/bin/bash

cd ..

echo "Compiling server..."
cmake -S server -B server/build
make -C server/build

echo "Creating database..."
sudo service mysql start
sudo mysql -u root < setup/imagic.sql

echo "Compiling load balancer..."
cmake -S load_balancer -B load_balancer/build
make -C load_balancer/build

echo "Starting server..."
./server/build/server &

echo "Waiting for server to be ready..."
sleep 2

echo "Starting load balancer..."
./load_balancer/build/load_balancer &

echo "Waiting for load balancer to be ready..."
sleep 2

echo "Executing client..."
python3 client/main.py

