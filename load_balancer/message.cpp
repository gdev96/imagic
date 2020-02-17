#include <netinet/in.h>
#include "message.h"

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

std::ostream & operator<<(std::ostream &os, const header &header) {
    os << "message_type: " << (unsigned int)header.message_type_ << " request_id: " << header.request_id_ << " payload_length: " << header.payload_length_;
    return os;
}

message::~message() {
    delete header_;
    delete[] payload_;
}
