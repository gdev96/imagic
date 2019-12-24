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
set +a

echo "Starting $N_SERVER servers..."
mkdir -p setup/generated_files
rm -f setup/generated_files/*.sql
ID=0
while [ $ID -lt $N_SERVER ]
do
    # source SQL statements
    . ./setup/imagic.sql

    echo "Creating database for server $ID..."
    echo "$STATEMENTS" > setup/generated_files/$ID.sql
    sudo mysql -u root < setup/generated_files/$ID.sql

    echo "Creating directory for server $ID..."
    mkdir -p server/resources/$ID
    ./server/build/server $ID &
    ((ID++))

    sleep 1
done

echo "Starting load balancer..."
./load_balancer/build/load_balancer &

echo "Waiting for load balancer to be ready..."
sleep 2

echo "Executing client..."
python3 client/main.py
