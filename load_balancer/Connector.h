#ifndef IMAGIC_BACKEND_CONNECTOR_H
#define IMAGIC_BACKEND_CONNECTOR_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <queue>

#define BUFFER_SIZE 10000000
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 1000
#define QUEUE_LENGTH_CONNECTIONS 5
#define HEADER_LENGTH 9
#define PAYLOAD_LENGTH 4
#define SOURCE_ID_LENGTH 4
#define MESSAGE_TYPE_LENGTH 1

using namespace std;
typedef struct header header;

struct header{
    unsigned char message_type;
    unsigned char source_id[4];
    unsigned char payload_length[4];
};

class ConnectorClient{

    private:
        queue <unsigned char> * message_queue_pointer;
        int client_sockfd;

    public:
        explicit ConnectorClient(queue<unsigned char> *messageQueuePointer);
        int getClientSockfd() const;
        int readHeader(unsigned char header[HEADER_LENGTH]);
        void receive();
};

class ConnectorServer{

    private:
        int client_sockfd, server_sockfd;
        struct sockaddr_in* server_address;
        unsigned int server_load;
        unsigned char connector_buffer[BUFFER_SIZE];

    public:
        ConnectorServer(int clientSockfd, sockaddr_in *serverAddress);
        [[nodiscard]] const unsigned char *getConnectorBuffer() const;
        [[nodiscard]] unsigned int getServerLoad() const;
        void setServerLoad(unsigned int serverLoad);
        void writeBuffer(unsigned char msg[], int n_bytes, int offset);
        int readSourceId(unsigned char source[]);
        int readPayloadLength(unsigned char source[]);
        void readMessage(unsigned char message[], int n_byte);
        void send();
};

int byteToInt(const unsigned char[], unsigned int n_bytes);
void pushBytes(queue<unsigned char>* q_pnt, unsigned char arr[],int n_bytes);

#endif //IMAGIC_BACKEND_CONNECTOR_H