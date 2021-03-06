#include <csignal>
#include <iostream>
#include "constants.h"
#include "load_balancer.h"

void termination_handler(int signal){
    std::cout << *OUTPUT_IDENTIFIER << "Stopping load balancer and servers..." << std::endl;
    exit(1);
}

int main() {
    // Manage SIGINT signals (i.e. CTRL+C)
    struct sigaction action;

    action.sa_handler = termination_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    sigaction(SIGINT, &action, nullptr);

    // Create and start load balancer
    load_balancer lb;
    lb.receive_requests();
}
