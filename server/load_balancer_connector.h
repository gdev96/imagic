#ifndef SERVER_LOAD_BALANCER_CONNECTOR_H
#define SERVER_LOAD_BALANCER_CONNECTOR_H

#include <cstdint>
#include <mutex>
#include <netinet/in.h>
#include "message.h"

inline uint32_t min(uint32_t a, uint32_t b) { return a<b ? a : b; }
void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void send(int sockfd, const message *msg);
message * receive(int sockfd);

class load_balancer_connector {
    struct sockaddr_in server_address_;
    unsigned int server_id_;
    message *current_message_;
    std::mutex *send_response_mutex_;
    void manage_request(int lb_sockfd, message *client_message);
public:
    load_balancer_connector() {}
    load_balancer_connector(const char *address, int port, unsigned int server_id);
    void receive_requests();
};

#endif //SERVER_LOAD_BALANCER_CONNECTOR_H
