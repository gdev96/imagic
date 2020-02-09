#ifndef IMAGIC_BACKEND_LOADBALANCER_H
#define IMAGIC_BACKEND_LOADBALANCER_H

#include <mutex>
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
        std::mutex read_mutex_, write_mutex_, write_count_mutex_;

    public:
        load_balancer();
        void initialize_server_addresses();
        void initialize_connectors();
        unsigned int balance();
        void get_requests();
        void manage_request(message *client_message);
};

#endif //IMAGIC_BACKEND_LOADBALANCER_H
