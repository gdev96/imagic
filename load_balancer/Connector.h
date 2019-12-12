#ifndef IMAGIC_BACKEND_CONNECTOR_H
#define IMAGIC_BACKEND_CONNECTOR_H

#include "Message.h"
#include "Constants.h"

class ConnectorClient{

    private:
        queue<Message> *message_queue;
        int client_sockfd;

    public:
        ConnectorClient(queue<Message> *message_queue);
        void manageRequest();
};

class ConnectorServer{

    private:
        int server_sockfd;
        struct sockaddr_in *server_address;
        unsigned int server_load;
        Message *message;

    public:
        ConnectorServer(sockaddr_in *serverAddress);
        const unsigned char *getConnectorBuffer() const;
        unsigned int getServerLoad() const;
        void setServerLoad(unsigned int serverLoad);
        void writeBuffer(unsigned char msg[], int n_bytes, int offset);
        int readSourceId(unsigned char source[]);
        int readPayloadLength(unsigned char source[]);
        void readMessage(unsigned char message[], int n_byte);
        void manageResponse(Message *message);
};

int byteToInt(const unsigned char[], unsigned int n_bytes);
void pushBytes(queue<unsigned char>* q_pnt, unsigned char arr[],int n_bytes);

#endif //IMAGIC_BACKEND_CONNECTOR_H