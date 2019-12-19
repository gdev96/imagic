#ifndef IMAGIC_BACKEND_CONNECTORS_H
#define IMAGIC_BACKEND_CONNECTORS_H

#include "message.h"
#include "constants.h"

uint32_t min(uint32_t a, uint32_t b);
void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);

class client_connector {
    private:
        queue<message *> *message_queue_;
        int client_sockfd_;
    public:
        client_connector(queue<message *> *message_queue);
        void manage_requests();
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
