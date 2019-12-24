#include <cstdlib>
#include <string>
#include "server.h"

int main(int argc, char **argv) {
    if(argc == 1) {
        throw std::runtime_error("Missing server id parameter");
    }
    int server_id = std::stoi(argv[1]);
    std::string server_address(std::getenv("SERVER_ADDRESS"));
    int server_port = std::stoi(std::getenv("SERVER_PORT"));
    server current_server = server(server_address, server_port + server_id, server_id);
    current_server.receive_requests();
}
