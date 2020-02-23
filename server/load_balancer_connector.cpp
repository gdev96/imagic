#include <arpa/inet.h>
#include <cstdint>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "constants.h"
#include "load_balancer_connector.h"
#include "storage_manager.h"

load_balancer_connector::load_balancer_connector(unsigned int server_id) : server_id_(server_id) {
    const char *server_address = std::getenv("SERVER_ADDRESS");
    int server_start_port = std::stoi(std::getenv("SERVER_START_PORT"));

    // Initialize IP address and port
    server_address_.sin_family = AF_INET;
    server_address_.sin_addr.s_addr = inet_addr(server_address);
    server_address_.sin_port = htons(server_start_port + server_id);

    send_response_mutex_ = new std::mutex();
}

void load_balancer_connector::read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
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

void load_balancer_connector::write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
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

void load_balancer_connector::send(int sockfd, const message *msg) {
    // Serialize response header
    unsigned char header_buffer[HEADER_LENGTH];
    msg->get_header()->serialize(header_buffer);

    // Send header
    write_bytes(sockfd, header_buffer, HEADER_LENGTH);

    // Serialize response payload
    uint32_t payload_length = msg->get_header()->get_payload_length();
    auto payload_buffer = new unsigned char[payload_length];
    msg->get_payload()->serialize(payload_buffer);

    // Send payload
    write_bytes(sockfd, payload_buffer, payload_length);

    delete[] payload_buffer;
}

message *load_balancer_connector::receive(int sockfd) {
    // Receive header
    unsigned char header_buffer[HEADER_LENGTH];
    read_bytes(sockfd, header_buffer, HEADER_LENGTH);

    // Deserialize request header
    auto message_header = new header();
    message_header->deserialize(header_buffer);

    // Receive payload
    uint32_t payload_length = message_header->get_payload_length();
    auto payload_buffer = new unsigned char[payload_length];
    read_bytes(sockfd, payload_buffer, payload_length);

    payload *message_payload;

    // Deserialize request payload
    if(message_header->get_message_type() == message_type::UPLOAD_IMAGE) {
        message_payload = new image_payload();
    }
    else {
        message_payload = new string_payload();
    }
    message_payload->deserialize(payload_buffer, payload_length);

    delete[] payload_buffer;

    return new message(message_header, message_payload);
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
    // Create storage manager
    storage_manager storage_manager_instance(client_message, server_id_);

    // Serve request
    switch(client_message->get_header()->get_message_type()) {
        case message_type::UPLOAD_IMAGE:
            storage_manager_instance.upload_image();
            break;
        case message_type::FIND_THUMBS:
            storage_manager_instance.find_thumbs();
            break;
        case message_type::DOWNLOAD_IMAGE:
            storage_manager_instance.download_image();
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
