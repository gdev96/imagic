#include <cstdlib>
#include <string>
#include "server.h"

int main() {
    int server_id = std::stoi(std::getenv("SERVER_ID"));

    server server_instance(server_id);
    server_instance.receive_requests();
}
