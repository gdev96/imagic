#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "constants.h"
#include "load_balancer_connector.h"

class server {
private:
    load_balancer_connector lb_connector_;
public:
    server(char *address, int port);
};

#endif //SERVER_SERVER_H
