#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
#include "class.hpp"


#define BUFFER_SIZE 10000000

using namespace std;

 
 /*
void send(Message * msg){    

    //BUFFER del connettore universale
    unsigned char buffer[BUFFER_SIZE];
    
    int sockfd, result, len;
    struct sockaddr_in address;

    //SOCKET CREATION
    sockfd = socket(AF_INET,SOCK_STREAM,0); 

    //SETTING CONNECTION PARAMETERS
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(msg->getHeader()->getSender()->getAddress().c_str); //IP address
    address.sin_port = htons(msg->getHeader()->getSender()->getPort()); //Port address
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr*)&address, len);  //Connect to Server (Application's client)

    //SEND THE MESSAGE

    //write(sockfd, &ch, 1); //Send request

    //SOCKET CLOSURE
    close(sockfd);  //Close the socket     

}

*/

/*void receive(){ //LB is Server

     char ch; 
    int server_sockfd, client_sockfd, result, server_len, client_len;
    struct sockaddr_in server_address, client_address;

    server_sockfd = socket(AF_INET,SOCK_STREAM,0);  //Create the socket 

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("151.97.6.4"); //IP address
    server_address.sin_port = htons(1000); //Port address
    server_len = sizeof(server_address);

    bind(server_sockfd,(struct sockaddr *)&server_address,server_len); //Connect socket to address/port
    listen(server_sockfd, 5); //Listen for client 

    client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address, (socklen_t*)client_len);

    read(client_sockfd, &ch, 1);

    close(client_sockfd);

} */