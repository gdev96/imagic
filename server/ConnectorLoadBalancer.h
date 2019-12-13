#ifndef SERVER_CONNECTORLOADBALANCER_H
#define SERVER_CONNECTORLOADBALANCER_H

#include "Message.h"
#include "Constants.h"

class ConnectorLoadBalancer {
private:
    queue<Message *> *message_queue;
    int lb_sockfd;
public:
    ConnectorLoadBalancer(queue<Message *> *messageQueue);
    void manageRequest();
};

#endif //SERVER_CONNECTORLOADBALANCER_H
