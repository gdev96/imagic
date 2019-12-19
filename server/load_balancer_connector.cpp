#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "constants.h"
#include "message.h"
#include "load_balancer_connector.h"

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
                throw std::runtime_error("Socket connection error");
            case 0:
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
                throw std::runtime_error("Socket connection error");
            case 0:
                throw std::runtime_error("Socket connection closed");
            default:
                offset += bytes_sent;
        }
    } while(offset < message_length);
}

load_balancer_connector::load_balancer_connector() {};

load_balancer_connector::load_balancer_connector(const char *address, int port, unsigned int server_id) {
    server_address_.sin_family = AF_INET;
    server_address_.sin_addr.s_addr = inet_addr(address);
    server_address_.sin_port = htons(port);
    server_id_ = server_id;
}

void load_balancer_connector::receive_requests() {
    struct sockaddr_in lb_address;

    //CREATE SOCKET
    int server_length = sizeof(server_address_);

    //CONNECTION WITH LOAD BALANCER
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *) &server_address_, server_length);
    listen(server_sockfd, QUEUE_LENGTH_CONNECTIONS);
    std::cout << "Load Balancer[" << server_id_ << "] connector is waiting for connections..." << std::endl;

    while (true) {
        int lb_length = sizeof(lb_address);
        int lb_sockfd = accept(server_sockfd, (struct sockaddr *) &lb_address, //every connector has a sockfd
                           reinterpret_cast<socklen_t *>(&lb_length));
        std::cout << "Connection accepted...Elaborating request..." << std::endl;

        std::thread t(&load_balancer_connector::manage_request, this, lb_sockfd);
        t.detach();
    }
}

void load_balancer_connector::manage_request(int lb_sockfd){
    //READ REQUEST FROM SOCKET AND CREATE MESSAGE
    unsigned char header_buffer[HEADER_LENGTH];
    read_bytes(lb_sockfd, header_buffer, HEADER_LENGTH);
    auto message_header = new header();
    message_header->deserialize(header_buffer);
    uint32_t payload_length = message_header->get_payload_length();
    message_type msg_type = message_header->get_message_type();
    auto message_payload = new payload();
    unsigned char payload_buffer[payload_length];
    read_bytes(lb_sockfd, payload_buffer, payload_length);
    message_payload->deserialize(payload_buffer, payload_length, msg_type);
    temporary_message_ = new message(message_header, message_payload);
    std::cout << "Message received...Processing request..." << std::endl;

    //MANAGE REQUEST
    storage_manager_ = new storage_manager(temporary_message_, server_id_);
    switch(msg_type) {
        case message_type::UPLOAD_IMAGE:
            storage_manager_->upload_request();
            break;
        case message_type::VIEW_THUMBS:
            storage_manager_->view_thumbs();
            break;
        case message_type::DOWNLOAD_IMAGE:
            storage_manager_->download_image();
            break;
    }
    //DELETE STORAGE MANAGER
    delete storage_manager_;

    //SERIALIZE RESPONSE HEADER
    unsigned char response_header_buffer[HEADER_LENGTH];
    temporary_message_->get_header()->serialize(response_header_buffer);

    //SEND HEADER
    write_bytes(lb_sockfd, response_header_buffer, HEADER_LENGTH);

    //SERIALIZE RESPONSE PAYLOAD
    uint32_t response_payload_length = temporary_message_->get_header()->get_payload_length();
    unsigned char response_payload_buffer[response_payload_length];
    temporary_message_->get_payload()->serialize(response_payload_buffer);

    //SEND PAYLOAD
    write_bytes(lb_sockfd, response_payload_buffer, response_payload_length);

    //DELETE MESSAGE
    delete temporary_message_;
}
