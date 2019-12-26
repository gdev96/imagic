#include <csignal>
#include <cstdlib>
#include <iostream>
#include "load_balancer.h"

void termination_handler(int signal){
    std::cout << "Stopping load balancer and servers..." << std::endl;
    exit(1);
}

int main() {
    //manage SIGINT signals (i.e. CTRL+C)
    struct sigaction action;

    action.sa_handler = termination_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, nullptr);

    load_balancer load_balancer_instance;
    load_balancer_instance.manage_requests();
}
