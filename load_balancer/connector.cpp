#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <thread>
#include "connector.h"
#include "constants.h"
#include "socket.h"

// Client connector

client_connector::client_connector(load_balancer *load_balancer) : load_balancer_(load_balancer) {
    const char *load_balancer_address = std::getenv("LOAD_BALANCER_ADDRESS");
    int load_balancer_port = std::stoi(std::getenv("LOAD_BALANCER_PORT"));

    // Initialize IP address and port
    lb_address_.sin_family = AF_INET;
    lb_address_.sin_addr.s_addr = inet_addr(load_balancer_address);
    lb_address_.sin_port = htons(load_balancer_port);
}

void client_connector::accept_connection_requests() {
    // Create socket to connect to client
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *)&lb_address_, sizeof(lb_address_));
    listen(server_sockfd, CONNECTIONS_QUEUE_SIZE);

    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);
    int client_sockfd;

    while(true) {
        std::cout << *OUTPUT_IDENTIFIER << "Waiting for connections from client..." << std::endl;

        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_length);

        std::cout << *OUTPUT_IDENTIFIER << "Connection from client accepted" << std::endl;

        std::thread t(&client_connector::queue_requests, this, client_sockfd);
        t.detach();
    }
}

void client_connector::queue_requests(int client_sockfd) {
    // Declare message
    message *received_message;

    while(true) {
        try {
            // Receive request
            received_message = receive(client_sockfd);
        } catch(const std::runtime_error &e) {
            std::cout << *OUTPUT_IDENTIFIER << e.what() << std::endl;
            break;
        }
        // Set message source id
        received_message->get_header()->set_request_id(current_request_id_);

        // Populate maps
        load_balancer_->request_map_[current_request_id_].push_back(client_sockfd);
        if(received_message->get_header()->get_message_type() == message_type::UPLOAD_IMAGE) {
            // Push number of remaining upload requests
            load_balancer_->request_map_[current_request_id_].push_back(load_balancer_->n_server_);

            // Push number of remaining upload responses
            load_balancer_->request_map_[current_request_id_].push_back(load_balancer_->n_server_);
        }
        current_request_id_++;

        // Increase write counter
        {
            std::scoped_lock lock(load_balancer_->write_count_mutex_);
            write_count_++;
        }

        // Write operation
        {
            std::scoped_lock lock(load_balancer_->write_mutex_);
            load_balancer_->message_queue_.push(received_message);
        }

        // Decrease write counter
        {
            std::scoped_lock lock(load_balancer_->write_count_mutex_);
            write_count_--;
            if(write_count_ == 0) {
                load_balancer_->notified = true;
                load_balancer_->message_production_.notify_one();
            }
        }
        std::cout << *OUTPUT_IDENTIFIER << "NEW MESSAGE RECEIVED AND QUEUED!" << std::endl;
        std::cout << *OUTPUT_IDENTIFIER << *received_message->get_header() << std::endl;
    }
}

// Server connector

std::mutex server_connector::request_map_mutex_ = std::mutex();

server_connector::server_connector(sockaddr_in *server_address, std::unordered_map<uint32_t, std::vector<int>> *request_map) : server_address_(server_address), request_map_(request_map) {
    send_request_mutex_ = new std::mutex();
    receive_response_mutex_ = new std::mutex();
    server_load_mutex_ = new std::mutex();

    // Connection with server
    server_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(server_sockfd_, (struct sockaddr *)server_address_, sizeof(*server_address_)) == -1) {
        throw std::runtime_error("Socket connection refused");
    }
}

void server_connector::increment_server_load() {
    std::scoped_lock lock(*server_load_mutex_);
    server_load_++;
}

enum map_values : int {
    CLIENT_SOCKFD = 0,
    UPLOAD_REQUEST_COUNTER = 1,
    UPLOAD_RESPONSE_COUNTER = 2
};

void server_connector::serve_request(const message *client_message) {
    // Send request to server
    {
        std::scoped_lock lock(*send_request_mutex_);
        send(server_sockfd_, client_message);
    }

    // Get request info
    message_type request_message_type = client_message->get_header()->get_message_type();
    uint32_t request_id = client_message->get_header()->get_request_id();

    // Free request info
    if(request_message_type == message_type::UPLOAD_IMAGE) {
        int remaining_upload_requests;

        // Get upload request counter and decrement it
        {
            std::scoped_lock lock(request_map_mutex_);
            (*request_map_)[request_id][UPLOAD_REQUEST_COUNTER]--;
            remaining_upload_requests = (*request_map_)[request_id][UPLOAD_REQUEST_COUNTER];
        }

        if(!remaining_upload_requests) {
            delete client_message;
        }
    }
    else {
        delete client_message;
    }
    message *response;

    // Receive response from server
    {
        std::scoped_lock lock(*receive_response_mutex_);
        response = receive(server_sockfd_);
    }

    // Get response info
    message_type response_message_type = response->get_header()->get_message_type();
    uint32_t response_id = response->get_header()->get_request_id();

    // Manage response
    if(response_message_type == message_type::UPLOAD_IMAGE) {
        int remaining_upload_responses;

        // Get upload response counter and decrement it
        {
            std::scoped_lock lock(request_map_mutex_);
            (*request_map_)[response_id][UPLOAD_RESPONSE_COUNTER]--;
            remaining_upload_responses = (*request_map_)[response_id][UPLOAD_RESPONSE_COUNTER];
        }

        if(!remaining_upload_responses) {
            // Only the last server connector which has received an upload response must send response
            send_response(response);

            // Delete entry in request map
            std::scoped_lock lock(request_map_mutex_);
            request_map_->erase(response_id);
        }
    }
    else {
        // Every server connector can send response
        send_response(response);

        // Delete entry in request map
        std::scoped_lock lock(request_map_mutex_);
        request_map_->erase(response_id);
    }
    // Decrement server load
    std::scoped_lock lock(*server_load_mutex_);
    server_load_--;
}

void server_connector::send_response(const message *response) {
    // Get client sockfd
    uint32_t request_id = response->get_header()->get_request_id();
    int client_sockfd = (*request_map_)[request_id][CLIENT_SOCKFD];

    try {
        // Send response to client
        send(client_sockfd, response);
        std::cout << *OUTPUT_IDENTIFIER << "RESPONSE SENT!" << std::endl;
        std::cout << *OUTPUT_IDENTIFIER << *response->get_header() << std::endl;
    } catch(const std::runtime_error &e) {
        std::cout << *OUTPUT_IDENTIFIER << e.what() << std::endl;
    }
    // Delete response info
    delete response;
}
