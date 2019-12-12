#include <iostream>
#include "LoadBalancer.h"

int main() {
    auto *load_balancer = new LoadBalancer();
    load_balancer->manageRequests();
}
