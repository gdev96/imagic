#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <netinet/in.h>
#include "storage_manager.h"

class load_balancer_connector {
    private:
        struct sockaddr_in server_address_;
        storage_manager *storage_manager_;
    public:
        load_balancer_connector();
        load_balancer_connector(char *address, int port);
        void receive_requests();
        void manage_request(int lb_sockfd);
};

#endif //SERVER_LOAD_BALANCER_CONNECTOR_H
