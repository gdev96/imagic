#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <mutex>
#include <netinet/in.h>
#include "message.h"

class load_balancer_connector {
    struct sockaddr_in server_address_;
    unsigned int server_id_;
    int lb_sockfd_;
    message *current_message_;
    std::mutex *send_response_mutex_;
    static uint32_t min(uint32_t a, uint32_t b) { return a<b ? a : b; }
    static void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
    static void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
    static void send(int sockfd, const message *msg);
    static message *receive(int sockfd);
    void manage_request(message *client_message);
public:
    load_balancer_connector() {}
    load_balancer_connector(unsigned int server_id);
    void receive_requests();
};

#endif  // SERVER_LOAD_BALANCER_CONNECTOR_H
