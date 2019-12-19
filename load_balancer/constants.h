#ifndef IMAGIC_BACKEND_CONSTANTS_H
#define IMAGIC_BACKEND_CONSTANTS_H

//INCLUDES
#include <iostream>
#include <string>
#include <cstring>
#include <queue>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <pthread.h>
#include "message.h"

//NAMESPACES
using  namespace std;

//CONNECTOR
#define CONNECTOR_CLIENT_ADDRESS "127.0.0.1"
#define CONNECTOR_CLIENT_PORT 5000
#define CONNECTOR_SERVER_ADDRESS "127.0.0.1"
#define CONNECTOR_SERVER_FIRST_PORT 39990
#define CONNECTOR_SERVER_PORT_STRIDE 10
#define QUEUE_LENGTH_CONNECTIONS 5

//MESSAGE
#define CHUNK_SIZE 8192

//HEADER
#define HEADER_LENGTH 9

//LOAD BALANCER
#define N_SERVER 1
#define MAX_SERVER 10

#endif //IMAGIC_BACKEND_CONSTANTS_H
