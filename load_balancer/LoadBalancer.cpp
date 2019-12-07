#include "LoadBalancer.h"

void LoadBalancer::readHeader(header * hdr){

    hdr->message_type = message_queue.front();
    message_queue.pop();
    for(int i=4; i>0; i--) {
        hdr->source_id[i] = message_queue.front();
        message_queue.pop();
    }
    for(int i=8; i>4; i--) {
        hdr->payload_length[i] = message_queue.front();
        message_queue.pop();
    }
}

void LoadBalancer::readPayload(unsigned char pld[], int pld_len){

    for(int i=pld_len; i>0; i--){
        pld[i]=message_queue.front();
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

void messageCopyOnBufferConnector(header* hdr, unsigned char pld[], int pld_len, ConnectorServer* cs){

    cs->writeBuffer(0,hdr->message_type);
    for(int i=1; i<5; i++) cs->writeBuffer(i,hdr->source_id[i]);
    for(int i=6; i<10; i++) cs->writeBuffer(i,hdr->payload_length[i]);
    for(int i=10; i<(pld_len+10); i++) cs->writeBuffer(i, pld[i]);
}

int LoadBalancer::manageResponse() {

    while(true) {
        if(!message_queue.empty()) {
            // MESSAGE FROM QUEUE
            header hdr;
            readHeader(&hdr);
            int byte_pld_l = payloadLengthByteToInt(hdr.payload_length);
            unsigned char pld[byte_pld_l];
            readPayload(pld, byte_pld_l);

            if(hdr.message_type == 0){ //Message must be send in broadcast
                for(int i=0; i<N_SERVER; i++){
                    messageCopyOnBufferConnector(&hdr,pld,byte_pld_l,server_connector[i]);
                    server_connector[i]->send();
                }
            }
            else{ //Message must be send only to one server
                int choosen_server = balance();
                messageCopyOnBufferConnector(&hdr,pld,byte_pld_l,server_connector[choosen_server]);
                server_connector[choosen_server]->send();
            }
        }
    }
};