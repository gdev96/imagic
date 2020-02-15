#ifndef LOAD_BALANCER_MESSAGE_H
#define LOAD_BALANCER_MESSAGE_H

#include <cstdint>
#include <ostream>

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
    header() {};
    message_type get_message_type() const { return message_type_; };
    uint32_t get_request_id() const { return request_id_; };
    uint32_t get_payload_length() const { return payload_length_; };
    void set_request_id(uint32_t request_id) { request_id_ = request_id; };
    void serialize(unsigned char *buffer);
    void deserialize(unsigned char *buffer);
    friend std::ostream &operator<<(std::ostream &os, const header &header);
};

class message {
    header *header_;
    unsigned char *payload_;
public:
    message(header *header, unsigned char *payload) : header_(header), payload_(payload) {};
    ~message();
    header *get_header() const { return header_; };
    unsigned char *get_payload() const { return payload_; };
};

#endif //LOAD_BALANCER_MESSAGE_H
