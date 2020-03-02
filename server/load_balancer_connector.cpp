#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include "constants.h"
#include "load_balancer_connector.h"
#include "socket.h"

load_balancer_connector::load_balancer_connector() {
    const char *server_address = std::getenv("SERVER_ADDRESS");
    int server_start_port = std::stoi(std::getenv("SERVER_START_PORT"));

    // Initialize IP address and port
    server_address_.sin_family = AF_INET;
    server_address_.sin_addr.s_addr = inet_addr(server_address);
    server_address_.sin_port = htons(server_start_port + std::stoi(std::getenv("SERVER_ID")));

    send_response_mutex_ = new std::mutex();

    // Create storage manager
    storage_manager_ = new storage_manager();
}

void load_balancer_connector::receive_requests() {
    // Create socket to connect to load balancer
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *)&server_address_, sizeof(server_address_));
    listen(server_sockfd, CONNECTIONS_QUEUE_SIZE);

    struct sockaddr_in lb_address;
    socklen_t lb_length = sizeof(lb_address);

    std::cout << *OUTPUT_IDENTIFIER << "Waiting for connections from load balancer..." << std::endl;

    lb_sockfd_ = accept(server_sockfd, (struct sockaddr *)&lb_address, &lb_length);

    std::cout << *OUTPUT_IDENTIFIER << "Connection from load balancer accepted" << std::endl;

    while(true) {
        // Receive request
        current_message_ = receive(lb_sockfd_);

        std::cout << *OUTPUT_IDENTIFIER << "NEW MESSAGE RECEIVED!" << std::endl;
        std::cout << *OUTPUT_IDENTIFIER << *current_message_->get_header() << std::endl;
        std::cout << *OUTPUT_IDENTIFIER << "Processing request..." << std::endl;

        std::thread t(&load_balancer_connector::manage_request, this, current_message_);
        t.detach();
    }
}

void load_balancer_connector::manage_request(message *client_message){
    // Serve request
    switch(client_message->get_header()->get_message_type()) {
        case message_type::UPLOAD_IMAGE:
            storage_manager_->upload_image(client_message);
            break;
        case message_type::FIND_THUMBS:
            storage_manager_->find_thumbs(client_message);
            break;
        case message_type::DOWNLOAD_IMAGE:
            storage_manager_->download_image(client_message);
    }
    // Send response
    {
        std::scoped_lock lock(*send_response_mutex_);
        send(lb_sockfd_, client_message);
    }

    std::cout << *OUTPUT_IDENTIFIER << "RESPONSE SENT!" << std::endl;
    std::cout << *OUTPUT_IDENTIFIER << *client_message->get_header() << std::endl;

    // Delete client request
    delete client_message;
}
