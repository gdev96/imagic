#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <netinet/in.h>

class load_balancer_connector {
    private:
        struct sockaddr_in server_address_;
    public:
        load_balancer_connector();
        load_balancer_connector(char *address, int port);
        void receive_requests();
        void manage_request(int lb_sockfd);
        void download_image();
        void view_thumbs();
        void upload_request();
};

#endif //SERVER_LOAD_BALANCER_CONNECTOR_H
