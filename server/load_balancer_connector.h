#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <mutex>
#include <netinet/in.h>
#include "message.h"
#include "storage_manager.h"

class load_balancer_connector {
    struct sockaddr_in server_address_;
    int lb_sockfd_;
    message *current_message_;
    std::mutex *send_response_mutex_;
    storage_manager *storage_manager_;
    void manage_request(message *client_message);
public:
    load_balancer_connector();
    void receive_requests();
};

#endif  // SERVER_LOAD_BALANCER_CONNECTOR_H
