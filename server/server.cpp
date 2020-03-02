#include <cstdlib>
#include <iostream>
#include <string>
#include "constants.h"
#include "server.h"

server::server() {
    // Get server id
    id_ = std::stoi(std::getenv("SERVER_ID"));

    // Set output identifier
    OUTPUT_IDENTIFIER = new std::string("\033[" + std::to_string(id_ % 5 + 32) + "mserver " + std::to_string(id_) + "      |\033[m ");
    std::cout << *OUTPUT_IDENTIFIER << "Server " << id_ <<" created" << std::endl;

    // Create load balancer connector
    lb_connector_ = load_balancer_connector();
    std::cout << *OUTPUT_IDENTIFIER << "Load balancer connector " << id_ << " created" << std::endl;
}

void server::receive_requests() {
    lb_connector_.receive_requests();
}
