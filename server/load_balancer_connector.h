#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <mutex>
#include <netinet/in.h>
#include "message.h"

class load_balancer_connector {
    struct sockaddr_in server_address_;
    unsigned int server_id_;
    int lb_sockfd_;
    message *current_message_;
    std::mutex *send_response_mutex_;
    void manage_request(message *client_message);
public:
    load_balancer_connector() {}
    load_balancer_connector(unsigned int server_id);
    void receive_requests();
};

#endif  // SERVER_LOAD_BALANCER_CONNECTOR_H
