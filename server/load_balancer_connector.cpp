#include "load_balancer_connector.h"

#include "load_balancer_connector.h"

//CONNECTOR LOAD BALANCER SIDE

load_balancer_connector::load_balancer_connector(queue<message *> *message_queue) : message_queue_(message_queue) {}

void load_balancer_connector::manage_requests() {
    //CREATE SOCKET
    struct sockaddr_in server_address, lb_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(CONNECTOR_SERVER_ADDRESS);
    server_address.sin_port = htons(CONNECTOR_SERVER_FIRST_PORT);
    int server_length = sizeof(server_address);

    //CONNECTION WITH CLIENT
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *) &server_address, server_length);
    listen(server_sockfd, QUEUE_LENGTH_CONNECTIONS);
    cout << "Client connector is waiting for connections..." << endl;

    while (true) {
        int lb_length = sizeof(lb_address);
        lb_sockfd_ = accept(server_sockfd, (struct sockaddr *) &lb_address, //every connector has a sockfd
                           reinterpret_cast<socklen_t *>(&lb_length));
        cout << "Connection accepted...Elaborating request..." << endl;

        //READ AND PUSH REQUEST
        unsigned char buffer[HEADER_LENGTH];
        read(lb_sockfd_, buffer, HEADER_LENGTH);
        auto message_header = new header();
        message_header->deserialize(buffer);
        message_header->set_source_id(lb_sockfd_);
        uint32_t payload_length = message_header->get_payload_length();
        auto message_payload = new unsigned char[payload_length];
        read(lb_sockfd_, message_payload, payload_length);
        auto received_message = new message(message_header, message_payload);
        message_queue_->push(received_message);
    }
}
