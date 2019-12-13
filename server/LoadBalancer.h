#ifndef IMAGIC_BACKEND_LOADBALANCER_H
#define IMAGIC_BACKEND_LOADBALANCER_H

#include "Constants.h"
#include "Connectors.h"
#include "Message.h"

class LoadBalancer {
    private:
        Message *current_message;
        queue<Message *> message_queue;
        ConnectorClient *client_connector;
        ConnectorServer server_connector[N_SERVER];
        struct sockaddr_in server_address[MAX_SERVER];
        thread arrayThreads[N_SERVER+1];
    public:
        LoadBalancer();
        void initializeServerAddresses();
        int balance();
        void manageRequests();
        void initializeClientConnector();
        void initializeServerConnectors();
};

#endif //IMAGIC_BACKEND_LOADBALANCER_H
