#ifndef IMAGIC_BACKEND_CONNECTOR_H
#define IMAGIC_BACKEND_CONNECTOR_H

#include "Message.h"
#include "Constants.h"

typedef struct header header;

struct header{

    unsigned char message_type;
    unsigned char source_id[4];
    unsigned char payload_length[4];
};

class ConnectorClient{

    private:
        queue <Message> * message_queue_pointer;
        int client_sockfd;

    public:
        explicit ConnectorClient(queue<unsigned char> *messageQueuePointer);
        int getClientSockfd() const;
        int readHeader(unsigned char header[HEADER_LENGTH]);
        void manageRequest();
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
        void manageResponse();
};

int byteToInt(const unsigned char[], unsigned int n_bytes);
void pushBytes(queue<unsigned char>* q_pnt, unsigned char arr[],int n_bytes);

#endif //IMAGIC_BACKEND_CONNECTOR_H