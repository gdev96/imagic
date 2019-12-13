#include "Server.h"

Server::Server() {
    //INITIALIZE SERVER ADDRESS

    //CREATE LOAD BALANCER CONNECTOR
    initializeLoadBalancerConnector();

}

void Server::initializeLoadBalancerConnector() {
    lb_connector = new ConnectorLoadBalancer(&message_queue);
    cout << "Load balancer connector created..." << endl;
  /*  arrayThreads[0] = thread(&ConnectorClient::manageRequest, this->client_connector); */
}

void Server::manageRequests() {
    /*
    while(true) {
        if(!message_queue.empty()) {
            //GET MESSAGE FROM QUEUE
            current_message = message_queue.front();
            message_queue.pop();
            cout << "Message received:" << endl;
            cout << *current_message->getHeader() << endl;

            if(!current_message->getHeader()->getMessageType()) { //Message must be sent in broadcast
                for(int i=0; i<N_SERVER; i++){
                    server_connector[i].setServerLoad(server_connector[i].getServerLoad()+1);
                    arrayThreads[i+1] = thread(&ConnectorServer::manageResponse, this->server_connector[i], current_message);
                    arrayThreads[i+1].detach();
                }
            }
            else { //Message must be sent to one server only
                int chosen_server = balance();
                server_connector[chosen_server].setServerLoad(server_connector[chosen_server].getServerLoad()+1);
                arrayThreads[chosen_server+1] = thread(&ConnectorServer::manageResponse, this->server_connector[chosen_server], current_message);
                arrayThreads[chosen_server+1].detach();
            }
        }
    } */
};

