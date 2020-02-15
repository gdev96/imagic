#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "connectors.h"
#include "constants.h"

void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
    uint32_t offset = 0;
    int bytes_received;
    do {
        bytes_received = read(sockfd, buffer + offset, min(message_length - offset, CHUNK_SIZE));
        switch(bytes_received) {
            case -1:
                close(sockfd);
                throw std::runtime_error("Socket connection error");
            case 0:
                close(sockfd);
                throw std::runtime_error("Socket connection closed");
            default:
                offset += bytes_received;
        }
    } while(offset < message_length);
}

void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
    uint32_t offset = 0;
    int bytes_sent;
    do {
        bytes_sent = write(sockfd, buffer + offset, message_length);
        switch(bytes_sent) {
            case -1:
                close(sockfd);
                throw std::runtime_error("Socket connection error");
            case 0:
                close(sockfd);
                throw std::runtime_error("Socket connection closed");
            default:
                offset += bytes_sent;
        }
    } while(offset < message_length);
}

void send(int sockfd, const message *msg) {
    //Serialize response header
    unsigned char header_buffer[HEADER_LENGTH];
    msg->get_header()->serialize(header_buffer);

    //Send header
    write_bytes(sockfd, header_buffer, HEADER_LENGTH);

    //Send payload
    uint32_t payload_length = msg->get_header()->get_payload_length();
    write_bytes(sockfd, msg->get_payload(), payload_length);
}

message *receive(int sockfd) {
    //Receive header
    unsigned char header_buffer[HEADER_LENGTH];
    read_bytes(sockfd, header_buffer, HEADER_LENGTH);

    //Deserialize request header
    auto message_header = new header();
    message_header->deserialize(header_buffer);

    //Receive payload
    uint32_t payload_length = message_header->get_payload_length();
    auto message_payload = new unsigned char[payload_length];
    read_bytes(sockfd, message_payload, payload_length);

    return new message(message_header, message_payload);
}

//Client connector

client_connector::client_connector(unsigned int n_server, std::queue<message *> *message_queue, std::unordered_map<uint32_t, std::vector<int>> *request_map, std::mutex *read_mutex, std::mutex *write_mutex, std::mutex *write_count_mutex) : n_server_(n_server), message_queue_(message_queue), request_map_(request_map), read_mutex_(read_mutex), write_mutex_(write_mutex), write_count_mutex_(write_count_mutex) {
    current_request_id_ = 0;
}

void client_connector::accept_requests() {
    //Create socket
    struct sockaddr_in server_address, client_address;

    const char *load_balancer_address(std::getenv("LOAD_BALANCER_ADDRESS"));
    int load_balancer_port = std::stoi(std::getenv("LOAD_BALANCER_PORT"));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(load_balancer_address);
    server_address.sin_port = htons(load_balancer_port);
    int server_length = sizeof(server_address);

    //Connection with client
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *) &server_address, server_length);
    listen(server_sockfd, QUEUE_LENGTH_CONNECTIONS);
    std::cout << *OUTPUT_IDENTIFIER << "Waiting for connections from client..." << std::endl;

    while (true) {
        socklen_t client_length = sizeof(client_address);
        int client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, &client_length);

        std::thread t(&client_connector::queue_request, this, client_sockfd);
        t.detach();
    }
}

void client_connector::queue_request(int client_sockfd) {
    try {
        std::cout << *OUTPUT_IDENTIFIER << "Connection from client accepted" << std::endl;

        while(true) {
            //Receive request
            message *received_message = receive(client_sockfd);

            //Set message source id
            received_message->get_header()->set_request_id(current_request_id_);

            //Populate maps
            (*request_map_)[current_request_id_].push_back(client_sockfd);
            if(received_message->get_header()->get_message_type() == message_type::UPLOAD_IMAGE) {
                //Push number of remaining upload request
                (*request_map_)[current_request_id_].push_back(n_server_);

                //Push number of remaining upload response
                (*request_map_)[current_request_id_].push_back(n_server_);
            }
            current_request_id_++;

            //Increase write_counter
            write_count_mutex_->lock();
            write_count_++;
            write_count_mutex_->unlock();

            write_mutex_->lock();

            //Write operation
            message_queue_->push(received_message);

            write_mutex_->unlock();

            //Decrease write_counter
            write_count_mutex_->lock();
            write_count_--;
            if(write_count_ == 0)
                read_mutex_->unlock();
            write_count_mutex_->unlock();

            std::cout << *OUTPUT_IDENTIFIER << "NEW MESSAGE RECEIVED AND QUEUED!" << std::endl;
            std::cout << *OUTPUT_IDENTIFIER << *received_message->get_header() << std::endl;
        }
    }
    catch (const std::runtime_error& e) {
        close(client_sockfd);
        std::cout << *OUTPUT_IDENTIFIER << e.what() << std::endl;
        return;
    }
}

//Server connector

server_connector::server_connector(sockaddr_in *server_address, std::unordered_map<uint32_t, std::vector<int>> *request_map) : server_address_(server_address), request_map_(request_map) {
    server_load_ = 0;
    send_request_mutex_ = new std::mutex();
    receive_response_mutex_ = new std::mutex();
    server_load_mutex_ = new std::mutex();

    //Connection with server
    server_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    unsigned int len = sizeof(*server_address_);
    if(connect(server_sockfd_, (struct sockaddr *)server_address_, len) == -1) {
        throw std::runtime_error("Socket connection refused");
    }
}

void server_connector::serve_request(message *client_message) {
    //Send request to server
    send_request_mutex_->lock();
    send(server_sockfd_, client_message);
    send_request_mutex_->unlock();

    //Get request info
    message_type request_message_type = client_message->get_header()->get_message_type();
    int request_id = client_message->get_header()->get_request_id();

    //Free request info
    if(request_message_type == message_type::UPLOAD_IMAGE) {
        //Get upload request counter and decrement it
        request_map_mutex_.lock();
        (*request_map_)[request_id][UPLOAD_REQUEST_COUNTER]--;
        unsigned int remaining_upload_requests = (*request_map_)[request_id][UPLOAD_RESPONSE_COUNTER];
        request_map_mutex_.unlock();

        if(!remaining_upload_requests) {
            delete client_message;
        }
    }
    else {
        delete client_message;
    }
    //Receive response from server
    receive_response_mutex_->lock();
    message *response = receive(server_sockfd_);
    receive_response_mutex_->unlock();

    //Get response info
    message_type response_message_type = response->get_header()->get_message_type();
    int response_id = response->get_header()->get_request_id();

    //Manage response
    if(response_message_type == message_type::UPLOAD_IMAGE) {
        //Get upload response counter and decrement it
        request_map_mutex_.lock();
        (*request_map_)[response_id][UPLOAD_RESPONSE_COUNTER]--;
        unsigned int remaining_upload_responses = (*request_map_)[response_id][UPLOAD_RESPONSE_COUNTER];
        request_map_mutex_.unlock();

        if(!remaining_upload_responses) {
            //Only the last server connector which has received an upload response must send response
            send_response(response);

            std::lock_guard<std::mutex> lock(request_map_mutex_);

            //Delete entry in request map
            request_map_->erase(response_id);
        }
    }
    else {
        //Every server connector can send response
        send_response(response);

        std::lock_guard<std::mutex> lock(request_map_mutex_);

        //Delete entry in request map
        request_map_->erase(response_id);
    }
    //Decrement server load
    std::lock_guard<std::mutex> lock(*server_load_mutex_);
    server_load_--;
}

void server_connector::send_response(message *response) {
    //Get client sockfd
    uint32_t request_id = response->get_header()->get_request_id();
    int client_sockfd = (*request_map_)[request_id][CLIENT_SOCKFD];

    //Send response to client
    send(client_sockfd, response);

    std::cout << *OUTPUT_IDENTIFIER << "RESPONSE SENT!" << std::endl;
    std::cout << *OUTPUT_IDENTIFIER << *response->get_header() << std::endl;

    //Delete response info
    delete response;
}
