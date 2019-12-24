#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "load_balancer_connector.h"

class server {
    private:
        load_balancer_connector lb_connector_;
        unsigned int id_;
    public:
        server(std::string &address, int port, int id);
        void receive_requests();
};

#endif //SERVER_SERVER_H
