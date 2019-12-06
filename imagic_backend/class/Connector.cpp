#include "Connector.h"

//CONNECTOR SERVER-SIDE
void Connector::send() {

    //CONNECTION WITH SERVER
    int sockfd=socket(AF_INET, SOCK_STREAM, 0);
    unsigned int len = sizeof(server_address);
    connect(sockfd, (struct sockaddr *)&server_address, len);

    //ASSEMBLE REQUEST TO SERVER
    //Read Header
    hdr.message_type = connector_buffer[0];
    for(int i=1; i<5; i++){ //Fill two array with one cycle
        hdr.source_id[i] = connector_buffer[i];
        hdr.payload_length[i+4] = connector_buffer[i+4];
    }
    int byte_pld_l = payloadLengthByteToInt(hdr.payload_length);

    //Read Payload
    unsigned char payload[byte_pld_l];
    for(int i=0; i<byte_pld_l; i++){
        payload[i] = connector_buffer[i+9];
    }

    //SEND REQUEST TO SERVER
    write(sockfd, &hdr.message_type,1);
    for(int i=4; i>0; i--){
        write(sockfd, &hdr.source_id[i],1);
    }
    for(int i=8; i>4; i--){
        write(sockfd, &hdr.payload_length[i],1);
    }
    for(int i=byte_pld_l; i>8; i--){
        write(sockfd, &payload[i],1);
    }

    //ASSEMBLE RESPONSE TO CLIENT
    //Read Header
    read(sockfd, &(hdr.message_type), 1);
    read(sockfd, hdr.source_id, 4);
    read(sockfd, hdr.payload_length, 4);
    byte_pld_l = payloadLengthByteToInt(hdr.payload_length);

    //Read Payload
    unsigned char payload2[byte_pld_l];
    read(client_sockfd, payload2, byte_pld_l);

    //SEND RESPONSE TO CLIENT

    write(client_sockfd, &hdr.message_type,1);
    for(int i=4; i>0; i--){
        write(client_sockfd, &hdr.source_id[i],1);
    }
    for(int i=8; i>4; i--){
        write(client_sockfd, &hdr.payload_length[i],1);
    }
    for(int i=byte_pld_l; i>8; i--){
        write(client_sockfd, &payload2[i],1);
    }

    //CLOSE CONNECTION TO CLIENT
    close(client_sockfd);
}

//CONNECTOR CLIENT-SIDE
void Connector::receive(){

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

            //MANAGE REQUEST
            //Read Header
            read(client_sockfd, &(hdr.message_type), 1);
            read(client_sockfd, hdr.source_id, 4);
            read(client_sockfd, hdr.payload_length, 4);
            int byte_pld_l = payloadLengthByteToInt(hdr.payload_length);

            //Read Payload
            unsigned char payload[byte_pld_l];
            read(client_sockfd, payload, byte_pld_l);

            //Insert the entire message in the queue
            message_queue_pointer->push(hdr.message_type);
            pushBytes(message_queue_pointer, hdr.source_id);
            pushBytes(message_queue_pointer, hdr.payload_length);
            pushBytes(message_queue_pointer, payload);

        }

}

int Connector::payloadLengthByteToInt(unsigned char pld_l[4]){
    int((unsigned char)(pld_l[0]) << 24 |
        (unsigned char)(pld_l[1]) << 16 |
        (unsigned char)(pld_l[2]) << 8 |
        (unsigned char)(pld_l[3]));
}

void Connector::pushBytes(queue<unsigned char> *q_pnt, unsigned char *arr) {
    unsigned int arr_bytes = sizeof(arr);
    for(int i=arr_bytes; i>0; i--) q_pnt->push(arr[i]); //first push low significant byte
}

Connector::Connector(queue<unsigned char> *messageQueuePointer) : message_queue_pointer(messageQueuePointer) {}

Connector::Connector(int clientSockfd, const header &hdr, const sockaddr_in &serverAddress,
                     queue<unsigned char> *messageQueuePointer) : client_sockfd(clientSockfd),
                                                                  server_address(serverAddress),
                                                                  message_queue_pointer(messageQueuePointer) {}

const unsigned char *Connector::getConnectorBuffer() const {
    return connector_buffer;
}

