#include <iostream>
#include "constants.h"
#include "server.h"

server::server(std::string &address, int port, int id) : id_(id) {
    //SET MESSAGE IDENTIFIER
    message_identifier = new std::string("\033[36mserver " + std::to_string(id_) + "      |\033[m ");
    //CREATE LOAD BALANCER CONNECTOR
    std::cout << *message_identifier << "Server " << id_ <<" created" << std::endl;
    lb_connector_ = load_balancer_connector(address, port, id);
    std::cout << *message_identifier << "Load balancer connector " << id << " created" << std::endl;
}

void server::receive_requests(){
    lb_connector_.receive_requests();
};
