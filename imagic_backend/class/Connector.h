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
    unsigned char message_type; //1 uploadImmagine 2 ricercaMiniature 3 downloadImmagine
    unsigned char source_id[4];
    unsigned char payload_length[4];
};

class Connector{

    private:

        int client_sockfd;
        struct sockaddr_in server_address;
        queue <unsigned char> * message_queue_pointer;
        unsigned char connector_buffer[BUFFER_SIZE];
        header hdr; //support variable

    public:

    //Connector-Client
    explicit Connector(queue<unsigned char> *messageQueuePointer);

    //Connector-Server
    Connector(int clientSockfd, const header &hdr, const sockaddr_in &serverAddress,
              queue<unsigned char> *messageQueuePointer);

    void send();
    void receive();
    static int payloadLengthByteToInt(unsigned char[4]);
    static void pushBytes(queue<unsigned char>* q_pnt, unsigned char arr[]);

    [[nodiscard]] const unsigned char *getConnectorBuffer() const;

};

#endif //IMAGIC_BACKEND_CONNECTOR_H
