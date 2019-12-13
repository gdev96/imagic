#include "ConnectorLoadBalancer.h"

#include "ConnectorLoadBalancer.h"

//CONNECTOR LOAD BALANCER SIDE

ConnectorLoadBalancer::ConnectorLoadBalancer(queue<Message *> *messageQueue) : message_queue(messageQueue) {}

void ConnectorLoadBalancer::manageRequest() {
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
        lb_sockfd = accept(server_sockfd, (struct sockaddr *) &lb_address, //every connector has a sockfd
                           reinterpret_cast<socklen_t *>(&lb_length));
        cout << "Connection accepted...Elaborating request..." << endl;

        //READ AND PUSH REQUEST
        unsigned char buffer[HEADER_LENGTH];
        read(lb_sockfd, buffer, HEADER_LENGTH);
        auto header = new Header();
        header->deserialize(buffer);
        header->setSourceId(lb_sockfd);
        uint32_t payload_length = header->getPayloadLength();
        auto payload = new unsigned char[payload_length];
        read(lb_sockfd, payload, payload_length);
        auto message = new Message(header, payload);
        message_queue->push(message);
    }
}