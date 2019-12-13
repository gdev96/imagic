#include <iostream>

#include "load_balancer.h"

int main() {
    auto *load_balancer_instance = new load_balancer();
    load_balancer_instance->manage_requests();
}
