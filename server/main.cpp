#include <thread>
#include "server.h"
#include "constants.h"

int main() {
    server current_server = server(SERVER_ADDRESS, SERVER_PORT, SERVER_ID);
    current_server.receive_requests();
}
