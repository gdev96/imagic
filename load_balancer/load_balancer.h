#ifndef IMAGIC_BACKEND_LOADBALANCER_H
#define IMAGIC_BACKEND_LOADBALANCER_H

#include <queue>
#include <thread>
#include "connectors.h"
#include "constants.h"
#include "message.h"

class load_balancer {
    private:
        message *current_message_;
        std::queue<message *> message_queue_;
        unsigned int n_server_;
        client_connector *client_connector_;
        server_connector *server_connectors_;
        struct sockaddr_in *server_addresses_;
        std::thread *threads_;
    public:
        load_balancer();
        void initialize_server_addresses();
        void initialize_client_connector();
        void initialize_server_connectors();
        int balance();
        void manage_requests();
};

#endif //IMAGIC_BACKEND_LOADBALANCER_H
