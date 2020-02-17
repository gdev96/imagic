#include <cstdlib>
#include <string>
#include "server.h"

int main() {
    int server_id = std::stoi(std::getenv("SERVER_ID"));
    const char *server_address = std::getenv("SERVER_ADDRESS");
    int server_port = std::stoi(std::getenv("SERVER_START_PORT"));
    server server_instance(server_address, server_port + server_id, server_id);
    server_instance.receive_requests();
}
