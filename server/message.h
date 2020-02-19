#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <cstdint>
#include <ostream>
#include "payload.h"

enum class message_type : unsigned char {
    UPLOAD_IMAGE = 0,
    FIND_THUMBS = 1,
    DOWNLOAD_IMAGE = 2
};

class header {
    message_type message_type_;
    uint32_t request_id_;
    uint32_t payload_length_;
public:
    header() {}
    message_type get_message_type() const { return message_type_; }
    uint32_t get_request_id() const { return request_id_; }
    uint32_t get_payload_length() const { return payload_length_; }
    void set_payload_length(uint32_t payload_length) { payload_length_ = payload_length; }
    void serialize(unsigned char *buffer);
    void deserialize(unsigned char *buffer);
    friend std::ostream & operator<<(std::ostream &os, const header &header);
};

class message {
    header *header_;
    payload *payload_;
public:
    message() {}
    message(header *header, payload *payload) : header_(header), payload_(payload) {}
    ~message();
    header *get_header() const { return header_; }
    payload *get_payload() const { return payload_; }
    void set_payload(payload *payload) { payload_ = payload; }
};

#endif  // SERVER_MESSAGE_H
