#ifndef LOAD_BALANCER_SOCKET_H
#define LOAD_BALANCER_SOCKET_H

#include <cstdint>
#include "message.h"

inline uint32_t min(uint32_t a, uint32_t b) { return a<b ? a : b; }
void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length);
void send(int sockfd, const message *msg);
message *receive(int sockfd);

#endif  // LOAD_BALANCER_SOCKET_H
