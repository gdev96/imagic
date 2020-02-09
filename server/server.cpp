#include <iostream>
#include "constants.h"
#include "server.h"

server::server(const char *address, int port, unsigned int id) : id_(id) {
    //SET OUTPUT IDENTIFIER
    OUTPUT_IDENTIFIER = new std::string("\033[" + std::to_string(id_ % 5 + 32) + "mserver " + std::to_string(id_) + "      |\033[m ");
    //CREATE LOAD BALANCER CONNECTOR
    std::cout << *OUTPUT_IDENTIFIER << "Server " << id_ <<" created" << std::endl;
    lb_connector_ = load_balancer_connector(address, port, id);
    std::cout << *OUTPUT_IDENTIFIER << "Load balancer connector " << id << " created" << std::endl;
}

void server::receive_requests() {
    lb_connector_.receive_requests();
}
