#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <queue>
#include <unordered_map>
#include "connectors.h"
#include "message.h"

// Forward declarations
class client_connector;
class server_connector;

class load_balancer {
    message *current_message_;
    std::queue<message *> message_queue_;
    std::unordered_map<uint32_t, std::vector<int>> request_map_;
    unsigned int n_server_;
    client_connector *client_connector_;
    server_connector *server_connectors_;
    struct sockaddr_in *server_addresses_;
    std::mutex write_mutex_, write_count_mutex_;
    std::condition_variable message_production_;
    bool notified = false;
    void initialize_server_addresses();
    void initialize_connectors();
    unsigned int balance();
    void manage_request(const message *client_message);
public:
    load_balancer();
    void receive_requests();
    void get_requests();
    friend class client_connector;
};

#endif  // LOAD_BALANCER_H
