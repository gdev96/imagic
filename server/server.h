#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "constants.h"
#include "load_balancer_connector.h"

class server {
private:
    message *current_message_;
    queue<message *> message_queue_;
    load_balancer_connector *lb_connector_;
    struct sockaddr_in server_address_;

public:
    server();
    void manage_requests();
    void initialize();
};

#endif //SERVER_SERVER_H
