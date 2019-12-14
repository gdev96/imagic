#include <iostream>
#include "server.h"

server::server(char *address, int port) {
    //CREATE LOAD BALANCER CONNECTOR
    lb_connector_ = load_balancer_connector(address, port);
    std::cout << "Load balancer connector created.." << std::endl;
    lb_connector_.receive_requests();
}
