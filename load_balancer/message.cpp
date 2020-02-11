#include <netinet/in.h>
#include "message.h"

header::header() {}

message_type header::get_message_type() const {
    return message_type_;
}

uint32_t header::get_request_id() const {
    return request_id_;
}

uint32_t header::get_payload_length() const {
    return payload_length_;
}

void header::set_request_id(uint32_t request_id) {
    request_id_ = request_id;
}

void header::serialize(unsigned char *buffer) {
    *buffer++ = (unsigned char)message_type_;
    auto *int_buffer = (uint32_t *)buffer;
    *int_buffer++ = htonl(request_id_);
    *int_buffer = htonl(payload_length_);
}

void header::deserialize(unsigned char *buffer) {
    message_type_ = (message_type)(*buffer++);
    auto *int_buffer = (uint32_t *)buffer;
    request_id_ = ntohl(*int_buffer++);
    payload_length_ = ntohl(*int_buffer);
}

std::ostream &operator<<(std::ostream &os, const header &header) {
    os << "message_type: " << (unsigned int)header.message_type_ << " request_id: " << header.request_id_ << " payload_length: " << header.payload_length_;
    return os;
}

message::message(header *header, unsigned char *payload) : header_(header), payload_(payload) {}

message::~message() {
    delete header_;
    delete[] payload_;
}

header *message::get_header() const {
    return header_;
}

unsigned char *message::get_payload() const {
    return payload_;
}
