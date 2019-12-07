#include "Connector.h"

//CONNECTOR CLIENT SIDE
ConnectorClient::ConnectorClient(queue<unsigned char> *messageQueuePointer) : message_queue_pointer(
        messageQueuePointer) {}

int ConnectorClient::getClientSockfd() const {
    return client_sockfd;
}

void ConnectorClient::readHeader(header hdr){

    read(client_sockfd, &(hdr.message_type), 1);
    read(client_sockfd, hdr.source_id, 4);
    read(client_sockfd, hdr.payload_length, 4);
}

void ConnectorClient::pushRequest(header hdr, unsigned char payload[]){
    message_queue_pointer->push(hdr.message_type);
    pushBytes(message_queue_pointer, hdr.source_id);
    pushBytes(message_queue_pointer, hdr.payload_length);
    pushBytes(message_queue_pointer, payload);
};

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

        //READ REQUEST
        header hdr;
        readHeader(hdr);
        int byte_pld_l = payloadLengthByteToInt(hdr.payload_length);
        unsigned char payload[byte_pld_l];
        read(client_sockfd, payload, byte_pld_l);

        //PUSH REQUEST IN THE QUEUE
        pushRequest(hdr,payload);
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

void ConnectorServer::writeBuffer(int position, unsigned char value){
    connector_buffer[position] = value;
}

void ConnectorServer::readHeader(header * hdr){

    hdr->message_type = connector_buffer[0];
    for(int i=1; i<5; i++){ //Fill two array with one cycle
        hdr->source_id[i] = connector_buffer[i];
        hdr->payload_length[i+4] = connector_buffer[i+4];
    }
}

void ConnectorServer::readPayload(unsigned char pld[], int pld_len){

    for(int i=9; i<(pld_len+9); i++){
        pld[i] = connector_buffer[i+9];
    }
}

void ConnectorServer::sendMessage(header hdr, unsigned char pld[], int pld_len, int sockfd){

    write(sockfd, &hdr.message_type,1);
    for(int i=4; i>0; i--){
        write(sockfd, &hdr.source_id[i],1);
    }
    for(int i=8; i>4; i--){
        write(sockfd, &hdr.payload_length[i],1);
    }
    for(int i=pld_len; i>8; i--){
        write(sockfd, &pld[i],1);
    }
}

void ConnectorServer::receiveHeader(header hdr, int sockfd){
    read(sockfd, &(hdr.message_type), 1);
    read(sockfd, hdr.source_id, 4);
    read(sockfd, hdr.payload_length, 4);
}

void ConnectorServer::send() {

    //GET REQUEST FROM BUFFER
    header hdr;
    readHeader(&hdr);
    int byte_pld_l = payloadLengthByteToInt(hdr.payload_length);
    unsigned char payload[byte_pld_l];
    readPayload(payload,byte_pld_l);

    //SEND REQUEST TO SERVER
    sendMessage(hdr, payload, byte_pld_l, server_sockfd);

    //GET RESPONSE FROM SERVER
    receiveHeader(hdr,server_sockfd);
    byte_pld_l = payloadLengthByteToInt(hdr.payload_length);
    unsigned char payload2[byte_pld_l];
    read(client_sockfd, payload2, byte_pld_l);

    //SEND RESPONSE TO CLIENT
    sendMessage(hdr,payload2,byte_pld_l,sockfd);

    //CLOSE CONNECTION TO CLIENT
    close(client_sockfd);
}

//OTHER FUNCTIONS
int payloadLengthByteToInt(unsigned char pld_l[4]){
    int((unsigned char)(pld_l[0]) << 24 |
        (unsigned char)(pld_l[1]) << 16 |
        (unsigned char)(pld_l[2]) << 8 |
        (unsigned char)(pld_l[3]));
}

void pushBytes(queue<unsigned char> *q_pnt, unsigned char *arr) {
    unsigned int arr_bytes = sizeof(arr);
    for(int i=arr_bytes; i>0; i--) q_pnt->push(arr[i]); //first push low significant byte
}