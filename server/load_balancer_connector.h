#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <netinet/in.h>
#include "storage_manager.h"

uint32_t min(uint32_t a, uint32_t b);
void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);

class load_balancer_connector {
    private:
        struct sockaddr_in server_address_;
        storage_manager *storage_manager_;
        unsigned int server_id_;
        message *temporary_message_;

    public:
        load_balancer_connector();
        load_balancer_connector(const char *address, int port, unsigned int server_id);
        void receive_requests();
        void manage_request(int lb_sockfd);
};

#endif //SERVER_LOAD_BALANCER_CONNECTOR_H
