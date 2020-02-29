#include <unistd.h>
#include "constants.h"
#include "socket.h"

void read_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
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

void write_bytes(int sockfd, unsigned char *buffer, uint32_t message_length) {
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

void send(int sockfd, const message *msg) {
    // Serialize response header
    unsigned char header_buffer[HEADER_LENGTH];
    msg->get_header()->serialize(header_buffer);

    // Send header
    write_bytes(sockfd, header_buffer, HEADER_LENGTH);

    // Serialize response payload
    uint32_t payload_length = msg->get_header()->get_payload_length();
    auto payload_buffer = new unsigned char[payload_length];
    msg->get_payload()->serialize(payload_buffer);

    // Send payload
    write_bytes(sockfd, payload_buffer, payload_length);

    delete[] payload_buffer;
}

message *receive(int sockfd) {
    // Receive header
    unsigned char header_buffer[HEADER_LENGTH];
    read_bytes(sockfd, header_buffer, HEADER_LENGTH);

    // Deserialize request header
    auto message_header = new header();
    message_header->deserialize(header_buffer);

    // Receive payload
    uint32_t payload_length = message_header->get_payload_length();
    auto payload_buffer = new unsigned char[payload_length];
    read_bytes(sockfd, payload_buffer, payload_length);

    payload *message_payload;

    // Deserialize request payload
    if(message_header->get_message_type() == message_type::UPLOAD_IMAGE) {
        message_payload = new image_payload();
    }
    else {
        message_payload = new string_payload();
    }
    message_payload->deserialize(payload_buffer, payload_length);

    delete[] payload_buffer;

    return new message(message_header, message_payload);
}
