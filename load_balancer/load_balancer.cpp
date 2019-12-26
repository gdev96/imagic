#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include "constants.h"
#include "load_balancer.h"

load_balancer::load_balancer() {
    //SET OUTPUT IDENTIFIER
    OUTPUT_IDENTIFIER = new std::string("\033[33mload_balancer |\033[m ");

    //INITIALIZE ATTRIBUTES
    n_server_ = std::stoi(std::getenv("N_SERVER"));
    server_addresses_ = new struct sockaddr_in[n_server_];
    server_connectors_ = new server_connector[n_server_];
    threads_ = new std::thread[n_server_ + 1];

    //INITIALIZE SERVER ADDRESSES
    initialize_server_addresses();

    //CREATE CLIENT CONNECTOR
    initialize_client_connector();

    //CREATE SERVER CONNECTORS
    initialize_server_connectors();
}

void load_balancer::initialize_server_addresses() {
    const char *server_address(std::getenv("SERVER_ADDRESS"));
    int server_port = std::stoi(std::getenv("SERVER_START_PORT"));

    for(int i=0; i<n_server_; i++) {
        server_addresses_[i].sin_family = AF_INET;
        server_addresses_[i].sin_addr.s_addr = inet_addr(server_address);
        server_addresses_[i].sin_port = htons(server_port + i);
    }
    std::cout << *OUTPUT_IDENTIFIER << "Server addresses initialized" << std::endl;
}

void load_balancer::initialize_client_connector() {
    client_connector_ = new client_connector(&message_queue_);
    std::cout << *OUTPUT_IDENTIFIER << "Client connector created" << std::endl;
    threads_[0] = std::thread(&client_connector::accept_requests, client_connector_);
    threads_[0].detach();
}

void load_balancer::initialize_server_connectors() {
    for(int i=0; i<n_server_; i++) {
        server_connectors_[i] = server_connector(&server_addresses_[i]);
        std::cout << *OUTPUT_IDENTIFIER << "Server connector " << i << " created" << std::endl;
    }
}

int load_balancer::balance() {

    int lowest_load_server = 0, lowest_load = 0;
    for(int i=0; i<n_server_; i++) {
        if(server_connectors_[i].get_server_load() <= lowest_load)
            lowest_load_server = i;
    }
    std::cout << *OUTPUT_IDENTIFIER << "SENDING MESSAGE TO SERVER: " << lowest_load_server << std::endl;
    return lowest_load_server;
};

void load_balancer::manage_requests() {
    while(true) {
        if(!message_queue_.empty()) {
            //GET MESSAGE FROM QUEUE
            current_message_ = message_queue_.front();
            message_queue_.pop();

            if(!current_message_->get_header()->get_message_type()) { //message must be sent in broadcast
                for(int i=0; i<n_server_; i++){
                    server_connectors_[i].set_server_load(server_connectors_[i].get_server_load()+1);
                    threads_[i+1] = std::thread(&server_connector::manage_response, server_connectors_[i], current_message_);
                    threads_[i+1].detach();
                }
            }
            else { //message must be sent to one server only
                int chosen_server = balance();
                server_connectors_[chosen_server].set_server_load(server_connectors_[chosen_server].get_server_load()+1);
                threads_[chosen_server+1] = std::thread(&server_connector::manage_response, server_connectors_[chosen_server], current_message_);
                threads_[chosen_server+1].detach();
            }
        }
    }
};
