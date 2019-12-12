#include <iostream>
#include "Message.h"

int main() {
    /*LoadBalancer * load_balancer = new LoadBalancer();
    thread lb_thread=thread(&LoadBalancer::manageRequest, load_balancer);*/

    unsigned char buffer[9] = {1, 0, 0, 0, 2, 0, 0, 0, 1};
    auto header = new Header();
    header->deserialize(buffer);
    header->serialize(buffer);
    header->deserialize(buffer);
    std::cout << *header << std::endl;
}