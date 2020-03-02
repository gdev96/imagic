#ifndef SERVER_H
#define SERVER_H

#include "load_balancer_connector.h"

class server {
    unsigned int id_;
    load_balancer_connector lb_connector_;
public:
    server();
    void receive_requests();
};

#endif  // SERVER_H
