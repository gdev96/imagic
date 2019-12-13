#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include "message.h"
#include "constants.h"

class load_balancer_connector {
private:
    queue<message *> *message_queue_;
    int lb_sockfd_;
public:
    load_balancer_connector(queue<message *> *message_queue);
    void manage_requests();
};

#endif //SERVER_LOAD_BALANCER_CONNECTOR_H
