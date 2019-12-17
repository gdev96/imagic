#include <iostream>
#include "server.h"

server::server(char *address, int port, unsigned int id) {
    //CREATE LOAD BALANCER CONNECTOR
    lb_connector_ = load_balancer_connector(address, port, id);
    std::cout << "Load balancer connector created.." << std::endl;
    lb_connector_.receive_requests();
}
