#include <iostream>
#include <thread>
#include "server.h"
#include "constants.h"

int main() {
    server *servers[N_SERVER];

    //Create Servers
    for(int i=0; i< N_SERVER; i++){
        servers[i] = new server(CONNECTOR_ADDRESS, CONNECTOR_FIRST_PORT + i*CONNECTOR_PORT_STRIDE, i);
        std::thread t(&load_balancer_connector::receive_requests, servers[i]->get_lb_connector());
        t.detach();
    }
    while(true) {}
}
