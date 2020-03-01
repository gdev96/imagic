#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <cstdint>
#include "message.h"

void send(int sockfd, const message *msg);
message *receive(int sockfd);

#endif  // SERVER_SOCKET_H
