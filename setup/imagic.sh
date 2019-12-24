#!/bin/bash

cd ..

echo "Creating database..."
sudo service mysql start
sudo mysql -u root < setup/imagic.sql

echo "Compiling server..."
cmake -S server -B server/build
make -C server/build

echo "Compiling load balancer..."
cmake -S load_balancer -B load_balancer/build
make -C load_balancer/build

# export environment variables
set -a
. ./setup/imagic.env
set +a

echo "Starting $N_SERVER servers..."
VAR=0
while [ "$VAR" -lt "$N_SERVER" ]
do
    mkdir -p server/resources/"$VAR"
    ./server/build/server "$VAR" &
    ((VAR++))
    sleep 1
done

echo "Starting load balancer..."
./load_balancer/build/load_balancer &

echo "Waiting for load balancer to be ready..."
sleep 2

echo "Executing client..."
python3 client/main.py
