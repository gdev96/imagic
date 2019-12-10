#include "LoadBalancer.h"

void LoadBalancer::readBytes(unsigned char buffer[],int offset, int n_bytes){

    for(int i=offset; i<n_bytes; i++){
        buffer[i]=message_queue.front();
        message_queue.pop();
    }
}

LoadBalancer::LoadBalancer() {

    //INIZIALIZATION SERVER ADDRESSES
    server_address[0].sin_family = AF_INET;
    server_address[0].sin_addr.s_addr = inet_addr("localhost");
    server_address[0].sin_port = htons(1000);
    server_address[1].sin_family = AF_INET;
    server_address[1].sin_addr.s_addr = inet_addr("localhost");
    server_address[1].sin_port = htons(2000);
    server_address[2].sin_family = AF_INET;
    server_address[2].sin_addr.s_addr = inet_addr("localhost");
    server_address[2].sin_port = htons(3000);

    //CREATE CLIENT CONNECTOR
    client_connector = new ConnectorClient(&message_queue);
    client_connector -> receive();
    int client_sockfd = client_connector -> getClientSockfd();

    //CREATE SERVER CONNECTORS
    for(int i=0; i<N_SERVER; i++){
        server_connector[i] = new ConnectorServer(client_sockfd,&server_address[i]);
    }
}

int LoadBalancer::balance(){

    int server_low_load=0, low_load=0;
    for(int i=0; i<N_SERVER; i++) {
        if ((server_connector[i]->getServerLoad()) <= low_load)
            server_low_load = i;
    }
    return server_low_load;
};

void messageCopyOnBufferConnector(unsigned char message[], int offset, int n_bytes, ConnectorServer* cs){

    cs->writeBuffer(message, n_bytes, offset);
}

void LoadBalancer::manageRequest() {

    while(true) {
        if(!message_queue.empty()) {
            // MESSAGE FROM QUEUE
            unsigned char header[HEADER_LENGTH];
            readBytes(header, 0, HEADER_LENGTH);
            unsigned char msg_t;
            memcpy(&msg_t, header, MESSAGE_TYPE_LENGTH);
            int message_type = byteToInt(&msg_t, MESSAGE_TYPE_LENGTH);
            unsigned char p_len[PAYLOAD_LENGTH];
            memcpy(&p_len, header + MESSAGE_TYPE_LENGTH + SOURCE_ID_LENGTH, PAYLOAD_LENGTH);
            int byte_pld_l = byteToInt(p_len, PAYLOAD_LENGTH);
            unsigned char pld[byte_pld_l];
            readBytes(pld, 0, byte_pld_l);

            if(!msg_t){ //Message must be sent in broadcast
                for(int i=0; i<N_SERVER; i++){
                    server_connector[i]->setServerLoad(server_connector[i]->getServerLoad()+1);
                    messageCopyOnBufferConnector(header, 0, HEADER_LENGTH,server_connector[i]);
                    messageCopyOnBufferConnector(pld, HEADER_LENGTH, byte_pld_l,server_connector[i]);
                    server_connector[i]->send();
                }
            }
            else{ //Message must be sent only to one server
                int chosen_server = balance();
                messageCopyOnBufferConnector(header, 0, HEADER_LENGTH,server_connector[chosen_server]);
                messageCopyOnBufferConnector(pld, HEADER_LENGTH, byte_pld_l,server_connector[chosen_server]);
                server_connector[chosen_server]->send();
            }
        }
    }
};