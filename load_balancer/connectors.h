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
        std::mutex *read_mutex_, *write_mutex_, *write_count_mutex;
        inline static unsigned int write_count_ = 0;
    public:
        client_connector(std::queue<message *> *message_queue, std::mutex *read_mutex, std::mutex *write_mutex,
                        std::mutex *write_count_mutex);
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
        void manage_response(const message* client_message, bool send_upload_response = false);
};

#endif //IMAGIC_BACKEND_CONNECTORS_H
