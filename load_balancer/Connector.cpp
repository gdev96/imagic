#include "Connector.h"

//CONNECTOR CLIENT SIDE
ConnectorClient::ConnectorClient(queue<unsigned char> *messageQueuePointer) : message_queue_pointer(
        messageQueuePointer) {}

int ConnectorClient::getClientSockfd() const {
    return client_sockfd;
}


int ConnectorClient::readHeader(unsigned char header[HEADER_LENGTH]){
    unsigned char pld_len[PAYLOAD_LENGTH];
    read(client_sockfd, &header, HEADER_LENGTH);
    memcpy(pld_len, header + HEADER_LENGTH - PAYLOAD_LENGTH, PAYLOAD_LENGTH);
    return byteToInt(pld_len, PAYLOAD_LENGTH);
}

void ConnectorClient::receive(){

    //CREATE SOCKET
    struct sockaddr_in server_address, client_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_address.sin_port = htons(SERVER_PORT);
    int server_length = sizeof(server_address);

    //CONNECTION WITH CLIENT
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr *) &server_address, server_length);
    listen(server_sockfd, QUEUE_LENGTH_CONNECTIONS);

    while (true) {

        cout << "\nServer in waiting for connections...";
        int client_length = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *) &client_address, //every connector has a sockfd
                               reinterpret_cast<socklen_t *>(&client_length));
        cout << "\nConnection accepted...\n Elaborating response...";

        //READ AND PUSH REQUEST
        unsigned char header[HEADER_LENGTH];
        int byte_pld_l = readHeader(header);
        pushBytes(message_queue_pointer, header, byte_pld_l);
        unsigned char payload[byte_pld_l];
        read(client_sockfd, payload, byte_pld_l);
        pushBytes(message_queue_pointer, payload, byte_pld_l);
    }
}

//CONNECTOR SERVER-SIDE
ConnectorServer::ConnectorServer(int clientSockfd, sockaddr_in *serverAddress) : client_sockfd(clientSockfd),
                                                                                 server_address(serverAddress) {
    server_load = 0;
    for (int i=0; i<BUFFER_SIZE; i++){
        connector_buffer[i]=0;
    }
    //CONNECTION WITH SERVER
    server_sockfd=socket(AF_INET, SOCK_STREAM, 0);
    unsigned int len = sizeof(server_address);
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

int ConnectorServer::readSourceId(unsigned char source[]){
    unsigned char s_id[SOURCE_ID_LENGTH];
    memcpy(s_id, source + MESSAGE_TYPE_LENGTH, SOURCE_ID_LENGTH);
    return byteToInt(s_id, SOURCE_ID_LENGTH);
}

int ConnectorServer::readPayloadLength(unsigned char source[]){
    unsigned char p_len[PAYLOAD_LENGTH];
    memcpy(p_len, source + HEADER_LENGTH - PAYLOAD_LENGTH, PAYLOAD_LENGTH);
    return byteToInt(p_len, PAYLOAD_LENGTH);
}

void ConnectorServer::readMessage(unsigned char message[], int n_byte){
    memcpy(message, connector_buffer, n_byte);
}

void ConnectorServer::send() {

    //GET REQUEST FROM BUFFER
    client_sockfd = readSourceId(connector_buffer);
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

    //DECREMENT SERVER LOAD
    server_load--;

    //CLOSE CONNECTION TO CLIENT
    close(client_sockfd);
}

//OTHER FUNCTIONS

int byteToInt(const unsigned char bytes[], unsigned int n_bytes) {
   int integer_value = 0;
   for(int i = 0; i<n_bytes; i++){
       integer_value += bytes[i] << (n_bytes - i - 1) * 8;
   }
   return integer_value;
}

void pushBytes(queue<unsigned char> *q_pnt, unsigned char *arr, int n_bytes) {
    for(int i=0; i<n_bytes; i++) q_pnt->push(arr[i]);
}
