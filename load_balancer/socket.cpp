#include <unistd.h>
#include "constants.h"
#include "socket.h"

void send_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
    uint32_t offset = 0;
    int bytes_sent;
    do {
        bytes_sent = write(sockfd, buffer + offset, message_length);
        switch(bytes_sent) {
            case -1:
                close(sockfd);
                throw std::runtime_error("Socket connection error");
            case 0:
                close(sockfd);
                throw std::runtime_error("Socket connection closed");
            default:
                offset += bytes_sent;
        }
    } while(offset < message_length);
}

void receive_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
    uint32_t offset = 0;
    int bytes_received;
    do {
        bytes_received = read(sockfd, buffer + offset, min(message_length - offset, CHUNK_SIZE));
        switch(bytes_received) {
            case -1:
                close(sockfd);
                throw std::runtime_error("Socket connection error");
            case 0:
                close(sockfd);
                throw std::runtime_error("Socket connection closed");
            default:
                offset += bytes_received;
        }
    } while(offset < message_length);
}

void send(int sockfd, const message *msg) {
    // Serialize response header
    unsigned char header_buffer[HEADER_LENGTH];
    msg->get_header()->serialize(header_buffer);

    // Send header
    send_bytes(sockfd, header_buffer, HEADER_LENGTH);

    // Send payload
    uint32_t payload_length = msg->get_header()->get_payload_length();
    send_bytes(sockfd, msg->get_payload(), payload_length);
}

message *receive(int sockfd) {
    // Receive header
    unsigned char header_buffer[HEADER_LENGTH];
    receive_bytes(sockfd, header_buffer, HEADER_LENGTH);

    // Deserialize request header
    auto message_header = new header();
    message_header->deserialize(header_buffer);

    // Receive payload
    uint32_t payload_length = message_header->get_payload_length();
    auto message_payload = new unsigned char[payload_length];
    receive_bytes(sockfd, message_payload, payload_length);

    return new message(message_header, message_payload);
}
