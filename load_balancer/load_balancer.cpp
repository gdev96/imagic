#include <arpa/inet.h>
#include <iostream>
#include "constants.h"
#include "load_balancer.h"

load_balancer::load_balancer() {

    //INITIALIZATION SERVER ADDRESSES
    initialize_server_addresses();

    //CREATE CLIENT CONNECTOR
    initialize_client_connector();

    //CREATE SERVER CONNECTORS
    initialize_server_connectors();
}

void load_balancer::initialize_server_addresses() {
    server_address_[0].sin_family = AF_INET;
    server_address_[0].sin_addr.s_addr = inet_addr(CONNECTOR_SERVER_ADDRESS);
    server_address_[0].sin_port = htons(CONNECTOR_SERVER_FIRST_PORT + CONNECTOR_SERVER_PORT_STRIDE);
    for(int i=1; i<N_SERVER; i++) {
        server_address_[i].sin_family = AF_INET;
        server_address_[i].sin_addr.s_addr = inet_addr(CONNECTOR_SERVER_ADDRESS);
        server_address_[i].sin_port = htons(server_address_[i-1].sin_port + CONNECTOR_SERVER_PORT_STRIDE);
    }
    std::cout << LOAD_BALANCER << "Server addresses initialized" << std::endl;
}

void load_balancer::initialize_client_connector() {
    client_connector_ = new client_connector(&message_queue_);
    std::cout << LOAD_BALANCER << "Client connector created" << std::endl;
    threads_[0] = std::thread(&client_connector::manage_requests, client_connector_);
}

void load_balancer::initialize_server_connectors() {
    for(int i=0; i<N_SERVER; i++) {
        server_connector_[i] = server_connector(&server_address_[i]);
        std::cout << LOAD_BALANCER << "Server connector " << i << " created" << std::endl;
    }
}

int load_balancer::balance() {

    int lowest_load_server = 0, lowest_load = 0;
    for(int i=0; i<N_SERVER; i++) {
        if(server_connector_[i].get_server_load() <= lowest_load)
            lowest_load_server = i;
    }
    std::cout << LOAD_BALANCER << "SENDING MESSAGE TO SERVER: " << lowest_load_server << std::endl;
    return lowest_load_server;
};

void load_balancer::manage_requests() {
    while(true) {
        if(!message_queue_.empty()) {
            //GET MESSAGE FROM QUEUE
            current_message_ = message_queue_.front();
            message_queue_.pop();

            if(!current_message_->get_header()->get_message_type()) { //message must be sent in broadcast
                for(int i=0; i<N_SERVER; i++){
                    server_connector_[i].set_server_load(server_connector_[i].get_server_load()+1);
                    threads_[i+1] = std::thread(&server_connector::manage_response, server_connector_[i], current_message_);
                    threads_[i+1].detach();
                }
            }
            else { //message must be sent to one server only
                int chosen_server = balance();
                server_connector_[chosen_server].set_server_load(server_connector_[chosen_server].get_server_load()+1);
                threads_[chosen_server+1] = std::thread(&server_connector::manage_response, server_connector_[chosen_server], current_message_);
                threads_[chosen_server+1].detach();
            }
        }
    }
};
