#include "Message.h"
#include "Constants.h"

Header::Header() {

}

void Header::serialize(unsigned char *buffer) {
    *buffer = message_type;
    *((uint32_t *)(buffer + MESSAGE_TYPE)) = htonl(source_id);
    *((uint32_t *)(buffer + MESSAGE_TYPE + SOURCE_ID)) = htonl(payload_length);
}

void Header::deserialize(const unsigned char *buffer) {
    message_type = *buffer;
    source_id = ntohl(*((uint32_t *)(buffer + MESSAGE_TYPE)));
    payload_length = ntohl(*((uint32_t *)(buffer + MESSAGE_TYPE + SOURCE_ID)));
}

ostream &operator<<(ostream &os, const Header &header) {
    os << "message_type: " << (uint16_t)header.message_type << " source_id: " << header.source_id << " payload_length: "
       << header.payload_length;
    return os;
}

Message::Message(const Header &header, unsigned char *payload, uint32_t payload_length) : header(header) {
    this->payload = new unsigned char[payload_length];
    //COPY payload ARRAY CONTENT
}
