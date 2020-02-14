#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include "connectors.h"
#include "constants.h"
#include "message.h"

class load_balancer {
    private:
        message *current_message_;
        std::queue<message *> message_queue_;
        std::unordered_map<uint32_t, std::vector<int>> request_map_;
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

#endif //LOAD_BALANCER_H
