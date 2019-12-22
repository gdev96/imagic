#include <iostream>
#include "server.h"

server::server(const char *address, int port, unsigned int id) {
    //CREATE LOAD BALANCER CONNECTOR
    std::cout << "Server " << id <<" created..." << std::endl;
    lb_connector_ = load_balancer_connector(address, port, id);
    std::cout << "Load balancer connector " << id << " created.." << std::endl;
}

void server::receive_requests(){
    lb_connector_.receive_requests();
};
