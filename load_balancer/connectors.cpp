#include "connectors.h"

//CONNECTOR CLIENT SIDE

client_connector::client_connector(queue<message *> *message_queue) : message_queue_(message_queue) {}

void client_connector::manage_requests() {
    //CREATE SOCKET
    struct sockaddr_in server_address, client_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(CONNECTOR_CLIENT_ADDRESS);
    server_address.sin_port = htons(CONNECTOR_CLIENT_PORT);
    int server_length = sizeof(server_address);

    //CONNECTION WITH CLIENT
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *) &server_address, server_length);
    listen(server_sockfd, QUEUE_LENGTH_CONNECTIONS);
    cout << "Client connector is waiting for connections..." << endl;

    while (true) {
        int client_length = sizeof(client_address);
        client_sockfd_ = accept(server_sockfd, (struct sockaddr *) &client_address, //every connector has a sockfd
                               reinterpret_cast<socklen_t *>(&client_length));
        cout << "Connection accepted...Elaborating request..." << endl;

        //READ AND PUSH REQUEST
        unsigned char buffer[HEADER_LENGTH];
        read(client_sockfd_, buffer, HEADER_LENGTH);
        auto message_header = new header();
        message_header->deserialize(buffer);
        message_header->set_source_id(client_sockfd_);
        uint32_t payload_length = message_header->get_payload_length();
        auto message_payload = new unsigned char[payload_length];
        read(client_sockfd_, message_payload, payload_length);
        auto received_message = new message(message_header, message_payload);
        message_queue_->push(received_message);
    }
}

//CONNECTOR SERVER-SIDE
server_connector::server_connector(){};
server_connector::server_connector(sockaddr_in *server_address) : server_address_(server_address) {
    server_load_ = 0;

    //CONNECTION WITH SERVER
    server_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    unsigned int len = sizeof(*server_address_);
    connect(server_sockfd_, (struct sockaddr *)&server_address_, len);
}

unsigned int server_connector::get_server_load() const {
    return server_load_;
}

void server_connector::set_server_load(unsigned int server_load) {
    server_load_ = server_load;
}

void server_connector::manage_response(message *message) { //Connector server know the current_message
    auto header = message->get_header();

    //GET THE CLIENT_SOCKFD
    uint32_t client_sockfd = header->get_source_id();
    uint32_t payload_length = header->get_payload_length();

    //SEND REQUEST TO SERVER
    unsigned char buffer[HEADER_LENGTH];
    header->serialize(buffer);
    write(server_sockfd_, buffer, HEADER_LENGTH);
    write(server_sockfd_, message->get_payload(), payload_length);

    //GET RESPONSE FROM SERVER
    read(server_sockfd_, buffer, HEADER_LENGTH);
    auto received_header = message->get_header();
    received_header->deserialize(buffer);
    read(server_sockfd_, buffer, received_header->get_payload_length());

    //SEND RESPONSE TO CLIENT
    write(client_sockfd, received_header,HEADER_LENGTH);
    write(client_sockfd, buffer, received_header->get_payload_length());

    //DELETE MESSAGE
    delete message;

    //DECREMENT SERVER LOAD
    server_load_--;

    //CLOSE CONNECTION TO CLIENT
    close(client_sockfd);
}
