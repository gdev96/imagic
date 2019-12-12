#include "Connectors.h"

//CONNECTOR CLIENT SIDE

ConnectorClient::ConnectorClient(queue<Message *> *messageQueue) : message_queue(messageQueue) {}

void ConnectorClient::manageRequest() {
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
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, //every connector has a sockfd
                               reinterpret_cast<socklen_t *>(&client_length));
        cout << "Connection accepted...Elaborating request..." << endl;

        //READ AND PUSH REQUEST
        unsigned char buffer[HEADER_LENGTH];
        read(client_sockfd, buffer, HEADER_LENGTH);
        auto header = new Header();
        header->deserialize(buffer);
        header->setSourceId(client_sockfd);
        uint32_t payload_length = header->getPayloadLength();
        auto payload = new unsigned char[payload_length];
        read(client_sockfd, payload, payload_length);
        auto message = new Message(header, payload);
        message_queue->push(message);
    }
}

//CONNECTOR SERVER-SIDE
ConnectorServer::ConnectorServer(sockaddr_in *serverAddress) : server_address(serverAddress) {
    server_load = 0;

    //CONNECTION WITH SERVER
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    unsigned int len = sizeof(*server_address);
    connect(server_sockfd, (struct sockaddr *)&server_address, len);
}

unsigned int ConnectorServer::getServerLoad() const {
    return server_load;
}

void ConnectorServer::setServerLoad(unsigned int serverLoad) {
    server_load = serverLoad;
}

void ConnectorServer::manageResponse(Message *message) { //Connector server know the current_message
    //GET THE CLIENT_SOCKFD
    uint32_t client_sockfd = message->getHeader()->getSourceId();
    uint32_t payload_length = message->getHeader()->getPayloadLength();

    //SEND REQUEST TO SERVER
    unsigned char buffer[HEADER_LENGTH];
    message->getHeader()->deserialize(buffer);
    write(server_sockfd, buffer, HEADER_LENGTH);
    write(server_sockfd, message->getPayload(), payload_length);

    //GET RESPONSE FROM SERVER
    read(server_sockfd, buffer, HEADER_LENGTH);
    message->getHeader()->deserialize(buffer);
    read(server_sockfd, buffer, message->getHeader()->getPayloadLength());

    //SEND RESPONSE TO CLIENT
    write(client_sockfd, message->getHeader(),HEADER_LENGTH);
    write(client_sockfd, buffer, message->getHeader()->getPayloadLength());

    //DELETE MESSAGE
    delete message;

    //DECREMENT SERVER LOAD
    server_load--;

    //CLOSE CONNECTION TO CLIENT
    close(client_sockfd);
}
