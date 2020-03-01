#ifndef LOAD_BALANCER_SOCKET_H
#define LOAD_BALANCER_SOCKET_H

#include <cstdint>
#include "message.h"

void send(int sockfd, const message *msg);
message *receive(int sockfd);

#endif  // LOAD_BALANCER_SOCKET_H
