#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include "constants.h"
#include "load_balancer.h"

load_balancer::load_balancer() {
    //Set output identifier
    OUTPUT_IDENTIFIER = new std::string("\033[31mload_balancer |\033[m ");

    //Initialize attributes
    n_server_ = std::stoi(std::getenv("N_SERVER"));
    server_addresses_ = new struct sockaddr_in[n_server_];
    server_connectors_ = new server_connector[n_server_];

    //Initialize server addresses
    initialize_server_addresses();

    //Create connectors
    initialize_connectors();
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

void load_balancer::initialize_connectors() {
    //Create client connector
    client_connector_ = new client_connector(&message_queue_, &read_mutex_, &write_mutex_, &write_count_mutex_);
    std::cout << *OUTPUT_IDENTIFIER << "Client connector created" << std::endl;
    std::thread t = std::thread(&client_connector::accept_requests, client_connector_);
    t.detach();

    //Create server connectors
    for(int i=0; i<n_server_; i++) {
        server_connectors_[i] = server_connector(&server_addresses_[i]);
        std::cout << *OUTPUT_IDENTIFIER << "Server connector " << i << " created" << std::endl;
    }
}

unsigned int load_balancer::balance() {
    unsigned int current_lowest_load = server_connectors_[0].get_server_load(), current_lowest_load_server = 0;
    for(int i=1; i<n_server_; i++) {
        unsigned int current_load = server_connectors_[i].get_server_load();
        if(current_load < current_lowest_load){
            current_lowest_load = current_load;
            current_lowest_load_server = i;
        }
    }
    return current_lowest_load_server;
}

void load_balancer::get_requests() {
    while(true) {
        //Waiting for message production
        read_mutex_.lock();

        write_mutex_.lock();

        //Read operation
        while(!message_queue_.empty()) {
            //Get messages from queue
            current_message_ = message_queue_.front();
            message_queue_.pop();

            //Manage request
            std::thread t = std::thread(&load_balancer::manage_request, this, current_message_);
            t.detach();
        }
        write_mutex_.unlock();
    }
}

void load_balancer::manage_request(message *client_message) {
    auto remaining_uploads = new unsigned int(n_server_);
    if(client_message->get_header()->get_message_type() == message_type::UPLOAD_IMAGE) {
        //Broadcast message
        std::cout << *OUTPUT_IDENTIFIER << "BROADCASTING MESSAGE" << std::endl;
        for(int i=0; i<n_server_; i++){
            server_connectors_[i].set_server_load(server_connectors_[i].get_server_load() + 1);
            std::thread t = std::thread(&server_connector::send_request_and_receive_response, server_connectors_[i], client_message, remaining_uploads);
            t.detach();
        }
    }
    else {
        //Send message to server with less load
        unsigned int chosen_server = balance();
        std::cout << *OUTPUT_IDENTIFIER << "SENDING MESSAGE TO SERVER: " << chosen_server << std::endl;
        server_connectors_[chosen_server].set_server_load(server_connectors_[chosen_server].get_server_load() + 1);
        server_connectors_[chosen_server].send_request_and_receive_response(client_message, remaining_uploads);
    }
}
