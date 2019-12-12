#include "Connector.h"
#include "Message.h"

//CONNECTOR CLIENT SIDE

ConnectorClient::ConnectorClient(queue<Message> *message_queue) : message_queue(message_queue) {}

void ConnectorClient::manageRequest(){

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

    while (true) {

        cout << "Connector-Client is waiting for connections..." << endl;
        int client_length = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, //every connector has a sockfd
                               reinterpret_cast<socklen_t *>(&client_length));
        cout << "\nConnection accepted...\n Elaborating response..." << endl;

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
        message_queue->push(*message);
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

const unsigned char *ConnectorServer::getConnectorBuffer() const {
    return connector_buffer;
}

unsigned int ConnectorServer::getServerLoad() const {
    return server_load;
}

void ConnectorServer::setServerLoad(unsigned int serverLoad) {
    server_load = serverLoad;
}

void ConnectorServer::writeBuffer(unsigned char msg[], int n_bytes, int offset){
    memcpy(connector_buffer + offset, msg, n_bytes);
}

void ConnectorServer::readMessage(unsigned char message[], int n_byte){
    memcpy(message, connector_buffer, n_byte);
}

void ConnectorServer::manageResponse(Message *message){

    //GET REQUEST FROM BUFFER
    uint32_t client_sockfd = message->getHeader()->getSourceId();
    unsigned char payload_length = readPayloadLength(connector_buffer);
    unsigned char message[HEADER_LENGTH+payload_length];
    readMessage(message, HEADER_LENGTH+payload_length);

    //SEND REQUEST TO SERVER
    write(server_sockfd, message, HEADER_LENGTH+payload_length);

    //GET RESPONSE FROM SERVER
    unsigned char header[HEADER_LENGTH];
    read(server_sockfd,header, HEADER_LENGTH);
    int byte_pld_l = readPayloadLength(header);
    unsigned char payload[byte_pld_l];
    read(server_sockfd, payload, byte_pld_l);

    //SEND RESPONSE TO CLIENT
    write(client_sockfd, header,HEADER_LENGTH);
    write(client_sockfd, payload, PAYLOAD_LENGTH);

    //DELETE MESSAGE
    delete message;

    //DECREMENT SERVER LOAD
    server_load--;

    //CLOSE CONNECTION TO CLIENT
    close(client_sockfd);
}