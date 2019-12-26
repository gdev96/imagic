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

# if no instance of load balancer process is running, kill eventually still active servers
if ! pgrep -x 'imagic_load_balancer' > /dev/null
then
    pkill -x 'imagic_server'
fi

# get number of active servers
SERVER_ID=$(pgrep -c 'imagic_server')

SERVERS_TO_CREATE=$(($N_SERVER-$SERVER_ID))

echo "Starting $SERVERS_TO_CREATE servers..."
echo "$SERVER_ID servers are already up-and-running!"

while [ $SERVER_ID -lt $N_SERVER ]
do
    # source SQL statements
    . ./setup/imagic.sql

    echo "Creating database for server $SERVER_ID..."
    sudo mysql -u root -e "$STATEMENTS"

    echo "Creating directory for server $SERVER_ID..."
    mkdir -p server/resources/$SERVER_ID
    ./server/build/imagic_server &
    ((SERVER_ID++))

    sleep 1
done

echo "Starting load balancer..."
# check if an instance of load balancer process is running
# if at least one server has been created, load balancer must be restarted
if ! pgrep -x 'imagic_load_balancer' > /dev/null || [ $SERVERS_TO_CREATE -gt 0 ]
then
    ./load_balancer/build/imagic_load_balancer &
else
    echo "Load balancer is already up-and-running!"
fi

echo "Waiting for load balancer to be ready..."
sleep 2

echo "Executing $N_CLIENT clients..."

#pip3 install -r client/requirements.txt

CLIENT_ID=0

while [ $CLIENT_ID -lt $N_CLIENT ]
do
    python3 client/main.py &
    ((CLIENT_ID++))

    sleep 1
done

# wait for processes termination
wait
