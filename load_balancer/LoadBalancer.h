#ifndef IMAGIC_BACKEND_LOADBALANCER_H
#define IMAGIC_BACKEND_LOADBALANCER_H

#include "Constants.h"
#include "Connector.h"
#include "Message.h"

class LoadBalancer{

    private:
        queue <Message> message_queue;
        ConnectorClient *client_connector;
        ConnectorServer *server_connector[N_SERVER];
        struct sockaddr_in server_address[MAX_SERVER];
        thread arrayThreads[N_SERVER+1];

    public:
        LoadBalancer();
        void initializeServerAddresses();
        int balance();
        void manageRequest();
        void readBytes(unsigned char buffer[], int offset, int n_bytes);
};

#endif //IMAGIC_BACKEND_LOADBALANCER_H