#ifndef IMAGIC_BACKEND_CONNECTORS_H
#define IMAGIC_BACKEND_CONNECTORS_H

#include <cstdint>
#include <netinet/in.h>
#include <queue>
#include "message.h"

u_int32_t min(uint32_t a, uint32_t b);
void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);

class client_connector {
    private:
        std::queue<message *> *message_queue_;
    public:
        client_connector(std::queue<message *> *message_queue);
        void accept_requests();
        void queue_request(int client_sockfd);
};

class server_connector {
    private:
        int server_sockfd_;
        struct sockaddr_in *server_address_;
        unsigned int server_load_;
    public:
        server_connector();
        server_connector(sockaddr_in *server_address);
        unsigned int get_server_load() const;
        void set_server_load(unsigned int server_load);
        void manage_response(message *message);
};

#endif //IMAGIC_BACKEND_CONNECTORS_H
