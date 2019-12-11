#include <iostream>
#include "LoadBalancer.h"

int main() {

    LoadBalancer * load_balancer = new LoadBalancer();
    thread lb_thread=thread(&LoadBalancer::manageRequest, load_balancer);
}
