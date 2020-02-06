#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "connectors.h"
#include "constants.h"

uint32_t min(uint32_t a, uint32_t b) {
    return (a<b)?a:b;
}

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

//Client-side connector

client_connector::client_connector(std::queue<message *> *message_queue, std::mutex *read_mutex, std::mutex *write_mutex,
                                   std::mutex *write_count_mutex) : message_queue_(message_queue), read_mutex_(read_mutex),
                                                                  write_mutex_(write_mutex),
                                                                  write_count_mutex(write_count_mutex) {}

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
            //Read and push request
            unsigned char buffer[HEADER_LENGTH];
            read_bytes(client_sockfd, buffer, HEADER_LENGTH);
            auto message_header = new header();
            message_header->deserialize(buffer);
            message_header->set_source_id(client_sockfd);
            uint32_t payload_length = message_header->get_payload_length();
            auto message_payload = new unsigned char[payload_length];
            read_bytes(client_sockfd, message_payload, payload_length);
            auto received_message = new message(message_header, message_payload);

            //Increase write_counter
            write_count_mutex->lock();
            write_count_++;
            write_count_mutex->unlock();

            write_mutex_->lock();

            //Write operation
            message_queue_->push(received_message);

            write_mutex_->unlock();

            //Decrease write_counter
            write_count_mutex->lock();
            write_count_--;
            if (write_count_==0)
                read_mutex_->unlock();
            write_count_mutex->unlock();

            std::cout << *OUTPUT_IDENTIFIER << "NEW MESSAGE RECEIVED AND QUEUED!" << std::endl;
            std::cout << *OUTPUT_IDENTIFIER << *message_header << std::endl;
        }
    }
    catch (const std::runtime_error& e) {
        close(client_sockfd);
        std::cout << *OUTPUT_IDENTIFIER << e.what() << std::endl;
        return;
    }
}

//Server-side connector
server_connector::server_connector() {};

server_connector::server_connector(sockaddr_in *server_address) : server_address_(server_address) {
    server_load_ = 0;

    //Connection with server
    server_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    unsigned int len = sizeof(*server_address_);
    if(connect(server_sockfd_, (struct sockaddr *)server_address_, len) == -1) {
        throw std::runtime_error("Socket connection refused");
    }
}

unsigned int server_connector::get_server_load() const {
    return server_load_;
}

void server_connector::set_server_load(unsigned int server_load) {
    server_load_ = server_load;
}

void server_connector::manage_response(const message *client_message, bool send_upload_response) {
    auto client_message_header = client_message->get_header();

    //Get the client_sockfd
    uint32_t client_sockfd = client_message_header->get_source_id();
    uint32_t client_payload_length = client_message_header->get_payload_length();

    //Send request to server
    unsigned char header_buffer[HEADER_LENGTH];
    client_message_header->serialize(header_buffer);
    write_bytes(server_sockfd_, header_buffer, HEADER_LENGTH);
    write_bytes(server_sockfd_, client_message->get_payload(), client_payload_length);

    //Get response from server
    read_bytes(server_sockfd_, header_buffer, HEADER_LENGTH);
    header server_message_header;
    server_message_header.deserialize(header_buffer);
    uint32_t server_payload_length = server_message_header.get_payload_length();
    unsigned char server_payload_buffer[server_payload_length];
    read_bytes(server_sockfd_, server_payload_buffer, server_payload_length);

    if(client_message_header->get_message_type() != message_type::UPLOAD_IMAGE || send_upload_response) {
        //Send response to client
        write_bytes(client_sockfd, header_buffer, HEADER_LENGTH);
        write_bytes(client_sockfd, server_payload_buffer, server_payload_length);
    }

    //Decrement server load
    server_load_--;
}
