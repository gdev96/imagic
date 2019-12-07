#ifndef IMAGIC_BACKEND_LOADBALANCER_H
#define IMAGIC_BACKEND_LOADBALANCER_H
#define N_SERVER 3
#define MAX_SERVER 10

#include <queue>
#include <iostream>
#include "Connector.h"

using namespace std;

class LoadBalancer{

    private:
        queue <unsigned char> message_queue;
        ConnectorClient *client_connector;
        ConnectorServer *server_connector[N_SERVER];
        struct sockaddr_in server_address[MAX_SERVER];

    public:
        LoadBalancer();
        int balance();
        int manageResponse();
        void readHeader(header * hdr);
        void readPayload(unsigned char pld[], int pld_len);
};

#endif //IMAGIC_BACKEND_LOADBALANCER_H
