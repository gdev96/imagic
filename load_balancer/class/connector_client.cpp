#include "connector_client.hpp"
#include<netinet/in.h>
#include <sys/socket.h>
#include<arpa/inet.h>
#include <unistd.h> 

void send(Message msg){    
    
    //Bisogna capire come gestire il messaggio nella write

    char ch;

    int sockfd, result, len;
    struct sockaddr_in address;

    sockfd = socket(AF_INET,SOCK_STREAM,0);  //Create the socket 

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("151.97.6.4"); //IP address
    address.sin_port = htons(1000); //Port address
    len = sizeof(address);

    result = connect(sockfd, (struct sockaddr*)&address, len);  //Connect to Server (our Client)

    write(sockfd, &ch, 1); //Send request

    close(sockfd);  //Close the socket     

}


void receive(){ //LB is Server

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

}