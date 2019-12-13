#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "Constants.h"
#include "ConnectorLoadBalancer.h"

class Server {
private:
    Message *current_message;
    queue<Message *> message_queue;
    ConnectorLoadBalancer *lb_connector;
    struct sockaddr_in server_address;

public:
    Server();
    void manageRequests();
    void initializeLoadBalancerConnector();
};

#endif //SERVER_SERVER_H
