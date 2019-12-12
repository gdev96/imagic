#include "Message.h"
#include "Constants.h"

Header::Header() {}

unsigned char Header::getMessageType() const {
    return message_type;
}

uint32_t Header::getSourceId() const {
    return source_id;
}

uint32_t Header::getPayloadLength() const {
    return payload_length;
}

void Header::setMessageType(unsigned char messageType) {
    message_type = messageType;
}

void Header::setSourceId(uint32_t sourceId) {
    source_id = sourceId;
}

void Header::setPayloadLength(uint32_t payloadLength) {
    payload_length = payloadLength;
}

void Header::serialize(unsigned char *buffer) {
    *buffer++ = message_type;
    auto *int_buffer = (uint32_t *)buffer;
    *int_buffer++ = htonl(source_id);
    *int_buffer = htonl(payload_length);
}

void Header::deserialize(unsigned char *buffer) {
    message_type = *buffer++;
    auto *int_buffer = (uint32_t *)buffer;
    source_id = ntohl(*int_buffer++);
    payload_length = ntohl(*int_buffer);
}

ostream &operator<<(ostream &os, const Header &header) {
    os << "message_type: " << (unsigned int)header.message_type << " source_id: " << header.source_id << " payload_length: "
       << header.payload_length;
    return os;
}

Message::Message(Header *header, unsigned char *payload) : header(header), payload(payload) {

}

Header *Message::getHeader() const {
    return header;
}

unsigned char *Message::getPayload() const {
    return payload;
}