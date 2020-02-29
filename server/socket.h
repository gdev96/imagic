#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <cstdint>
#include "message.h"

inline uint32_t min(uint32_t a, uint32_t b) { return a<b ? a : b; }
void send_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void receive_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void send(int sockfd, const message *msg);
message *receive(int sockfd);

#endif  // SERVER_SOCKET_H
