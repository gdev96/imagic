#!/bin/bash

cd ..

echo "Starting MySQL service..."
sudo service mysql start

echo "Compiling server..."
cmake -S server -B server/build
make -C server/build

echo "Compiling load balancer..."
cmake -S load_balancer -B load_balancer/build
make -C load_balancer/build

# export environment variables
set -a
. ./setup/imagic.env
SERVER_ID=0

# avoid exporting other environment variables
set +a

echo "Starting $N_SERVER servers..."
while [ $SERVER_ID -lt $N_SERVER ]
do
    # source SQL statements
    . ./setup/imagic.sql

    echo "Creating database for server $SERVER_ID..."
    sudo mysql -u root -e "$STATEMENTS"

    echo "Creating directory for server $SERVER_ID..."
    mkdir -p server/resources/$SERVER_ID
    ./server/build/server &
    ((SERVER_ID++))

    sleep 1
done

echo "Starting load balancer..."
./load_balancer/build/load_balancer &

echo "Waiting for load balancer to be ready..."
sleep 2

echo "Executing client..."
python3 client/main.py
