#include <netinet/in.h>
#include "message.h"

header::header() {}

message_type header::get_message_type() const {
    return message_type_;
}

uint32_t header::get_source_id() const {
    return source_id_;
}

uint32_t header::get_payload_length() const {
    return payload_length_;
}

void header::set_message_type(message_type msg_type) {
    message_type_ = msg_type;
}

void header::set_source_id(uint32_t source_id) {
    source_id_ = source_id;
}

void header::set_payload_length(uint32_t payload_length) {
    payload_length_ = payload_length;
}

void header::serialize(unsigned char *buffer) {
    *buffer++ = (unsigned char)message_type_;
    auto *int_buffer = (uint32_t *)buffer;
    *int_buffer++ = htonl(source_id_);
    *int_buffer = htonl(payload_length_);
}

void header::deserialize(unsigned char *buffer) {
    message_type_ = (message_type)(*buffer++);
    auto *int_buffer = (uint32_t *)buffer;
    source_id_ = ntohl(*int_buffer++);
    payload_length_ = ntohl(*int_buffer);
}

std::ostream &operator<<(std::ostream &os, const header &header) {
    os << "message_type: " << (unsigned int)header.message_type_ << " source_id: " << header.source_id_
        << " payload_length: " << header.payload_length_;
    return os;
}

message::message(header *header, unsigned char *payload) : header_(header), payload_(payload) {}

message::~message() {
    delete header_;
    delete payload_;
}

header *message::get_header() const {
    return header_;
}

unsigned char *message::get_payload() const {
    return payload_;
}
