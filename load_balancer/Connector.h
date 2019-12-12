#ifndef IMAGIC_BACKEND_CONNECTOR_H
#define IMAGIC_BACKEND_CONNECTOR_H

#include "Message.h"
#include "Constants.h"

class ConnectorClient{

    private:
        queue<Message *> *message_queue;
        int client_sockfd;

    public:
        ConnectorClient(queue<Message *> *messageQueue);
        void manageRequest();
};

class ConnectorServer{

    private:
        int server_sockfd;
        struct sockaddr_in *server_address;
        unsigned int server_load;

    public:
        ConnectorServer(sockaddr_in *serverAddress);
        unsigned int getServerLoad() const;
        void setServerLoad(unsigned int serverLoad);
        void manageResponse(Message *message);
};

#endif //IMAGIC_BACKEND_CONNECTOR_H