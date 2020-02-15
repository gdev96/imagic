#!/bin/bash

cd ..

echo "Starting MySQL service..."
sudo service mysql start

if [ "$1" != 0 ] && [ "$1" == "test" ]
then
    echo "Compiling server in testing mode..."
    cmake -DTESTING_MACRO=ON -S server -B server/build
else
    echo "Compiling server in normal mode..."
    cmake -DTESTING_MACRO=OFF -S server -B server/build
fi
make -C server/build

echo "Compiling load balancer..."
cmake -S load_balancer -B load_balancer/build
make -C load_balancer/build

# Export environment variables
set -a

. ./setup/imagic.env

read -p "Enter number of servers: " N_SERVER
while ! [[ $N_SERVER =~ ^[0-9]+$ ]] || [ $N_SERVER -eq 0 ]
do
    read -p "Sorry, enter a valid number: " N_SERVER
done

SERVER_ID=0

# Avoid exporting other environment variables
set +a

# Get number of active load balancers (name is truncated to 15 characters)
if pgrep -x 'imagic_lb' > /dev/null
then
    echo "$(pgrep -c -x 'imagic_lb') active load balancers found!"
    echo "Killing them..."
    pkill 'imagic_lb'
fi

# Get number of active servers
if pgrep -x 'imagic_srv' > /dev/null
then
    echo "$(pgrep -c -x 'imagic_srv') active servers found!"
    echo "Killing them..."
    pkill 'imagic_srv'
fi

echo "Starting $N_SERVER servers..."

while [ $SERVER_ID -lt $N_SERVER ]
do
    # Source SQL statements
    . ./setup/imagic.sql

    echo "Creating database for server $SERVER_ID..."
    sudo mysql -u root -e "$STATEMENTS"

    echo "Creating directory for server $SERVER_ID..."
    mkdir -p server/images/$SERVER_ID
    ./server/build/imagic_srv &
    ((SERVER_ID++))

    sleep 1
done

echo "Starting load balancer..."
./load_balancer/build/imagic_lb &

echo "Waiting for load balancer to be ready..."
sleep 2

read -p "Enter number of clients: " N_CLIENT
while ! [[ $N_CLIENT =~ ^[0-9]+$ ]] || [ $N_CLIENT -eq 0 ]
do
    read -p "Sorry, enter a valid number: " N_CLIENT
done

echo "Executing $N_CLIENT clients..."

CLIENT_ID=0

while [ $CLIENT_ID -lt $N_CLIENT ]
do
    python3 client/main.py &
    ((CLIENT_ID++))

    sleep 1
done

# Wait for processes termination
wait
