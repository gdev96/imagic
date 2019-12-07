#ifndef IMAGIC_BACKEND_CONNECTOR_H
#define IMAGIC_BACKEND_CONNECTOR_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <queue>

#define BUFFER_SIZE 10000000
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 1000
#define QUEUE_LENGTH_CONNECTIONS 5

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
        void readHeader(header hdr);
        void pushRequest(header hdr, unsigned char payload[]);
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
        void writeBuffer(int position, unsigned char value);
        void readHeader(header * hdr);
        void readPayload(unsigned char pld[], int pld_len);
        void sendMessage(header hdr, unsigned char pld[], int pld_len, int sockfd);
        void receiveHeader(header hdr, int sockfd);
        void send();
};

static int payloadLengthByteToInt(unsigned char[4]);
static void pushBytes(queue<unsigned char>* q_pnt, unsigned char arr[]);

#endif //IMAGIC_BACKEND_CONNECTOR_H