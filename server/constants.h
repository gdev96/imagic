#ifndef IMAGIC_BACKEND_CONSTANTS_H
#define IMAGIC_BACKEND_CONSTANTS_H

//INCLUDES
#include <cstring>
#include <iostream>
#include <string>
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
#define CONNECTOR_SERVER_FIRST_PORT 6000
#define CONNECTOR_SERVER_OFFSET_PORT 10
#define BUFFER_SIZE 10000000 //bytes
#define QUEUE_LENGTH_CONNECTIONS 5

//HEADER
#define HEADER_LENGTH 9
#define MESSAGE_TYPE 1
#define SOURCE_ID 4
#define PAYLOAD_LENGTH 4

//LOAD BALANCER
#define N_SERVER 3
#define MAX_SERVER 10

#endif //IMAGIC_BACKEND_CONSTANTS_H
