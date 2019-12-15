#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "constants.h"
#include "message.h"
#include "load_balancer_connector.h"

load_balancer_connector::load_balancer_connector() {};

load_balancer_connector::load_balancer_connector(char *address, int port) {
    server_address_.sin_family = AF_INET;
    server_address_.sin_addr.s_addr = inet_addr(address);
    server_address_.sin_port = htons(port);
}

void load_balancer_connector::receive_requests() {
    struct sockaddr_in lb_address;

    //CREATE SOCKET
    int server_length = sizeof(server_address_);

    //CONNECTION WITH CLIENT
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *) &server_address_, server_length);
    listen(server_sockfd, QUEUE_LENGTH_CONNECTIONS);
    std::cout << "Client connector is waiting for connections..." << std::endl;

    while (true) {
        int lb_length = sizeof(lb_address);
        int lb_sockfd_ = accept(server_sockfd, (struct sockaddr *) &lb_address, //every connector has a sockfd
                           reinterpret_cast<socklen_t *>(&lb_length));
        std::cout << "Connection accepted...Elaborating request..." << std::endl;

        std::thread t(&load_balancer_connector::receive_requests, this);

    }
}

void load_balancer_connector::manage_request(int lb_sockfd){
    //READ REQUEST FROM SOCKET AND CREATE MESSAGE
    unsigned char header_buffer[HEADER_LENGTH];
    read(lb_sockfd, header_buffer, HEADER_LENGTH);
    auto message_header = new header();
    message_header->deserialize(header_buffer);
    uint32_t payload_length = message_header->get_payload_length();
    unsigned char message_type = message_header->get_message_type();
    auto message_payload = new payload();
    unsigned char payload_buffer[payload_length];
    read(lb_sockfd, payload_buffer, payload_length);
    message_payload->deserialize(payload_buffer, payload_length, message_type);
    auto received_message = new message(message_header, message_payload);
    std::cout << "Message received...Processing request..." << std::endl;

    //MANAGE REQUEST
    storage_manager_ = new storage_manager(*received_message);
    switch(message_type) {
        case 0: //UPLOAD REQUEST -> save paths in db and files in storage
            storage_manager_->upload_request();
            break;
        case 1: //VIEW THUMBS -> get thumbs map and send response
            storage_manager_->view_thumbs();
            break;
        case 2: //DOWNLOAD IMAGE -> get the image to send response
            storage_manager_->download_image();
            break;
        default:
            break;
    }
}
