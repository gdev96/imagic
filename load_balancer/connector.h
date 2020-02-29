#ifndef LOAD_BALANCER_CONNECTOR_H
#define LOAD_BALANCER_CONNECTOR_H

#include <cstdint>
#include <mutex>
#include <netinet/in.h>
#include <unordered_map>
#include "load_balancer.h"
#include "message.h"

// Forward declaration
class load_balancer;

class client_connector {
    load_balancer *load_balancer_;
    struct sockaddr_in lb_address_;
    uint32_t current_request_id_ = 0;
    unsigned int write_count_ = 0;
    void queue_requests(int client_sockfd);
public:
    client_connector(load_balancer *load_balancer);
    void accept_connection_requests();
};

class server_connector {
    int server_sockfd_;
    struct sockaddr_in *server_address_;
    unsigned int server_load_ = 0;
    std::unordered_map<uint32_t, std::vector<int>> *request_map_;
    std::mutex *send_request_mutex_, *receive_response_mutex_, *server_load_mutex_;
    static std::mutex request_map_mutex_;
    void send_response(const message *response);
public:
    server_connector() {}
    server_connector(sockaddr_in *server_address, std::unordered_map<uint32_t, std::vector<int>> *request_map);
    unsigned int get_server_load() const { return server_load_; }
    void increment_server_load();
    void serve_request(const message *client_message);
};

#endif  // LOAD_BALANCER_CONNECTOR_H
