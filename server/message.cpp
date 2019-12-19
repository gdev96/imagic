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

message::message() {}

message::message(header *header, payload *payload) : header_(header), payload_(payload) {}

message::~message() {
    delete header_;
    delete[] payload_;
}

header *message::get_header() const {
    return header_;
}

payload *message::get_payload() const {
    return payload_;
}

void message::set_payload(payload *payload) {
    payload_ = payload;
}

const std::variant<image *, std::string *, std::vector<unsigned char> *, std::map<std::vector<unsigned char>, std::string> *> &
payload::get_content() const {
    return content_;
}

void payload::set_content(
        const std::variant<image *, std::string *, std::vector<unsigned char> *, std::map<std::vector<unsigned char>, std::string> *> &content) {
    content_ = content;
}

void payload::serialize(unsigned char *buffer) {
    if(content_.index() == 3) { //SERIALIZE MAP
        //GET MAP FROM VARIANT
        std::map<std::vector<unsigned char>, std::string> *thumbs_map = std::get<3>(content_);

        uint32_t *int_buffer;
        unsigned char *byte_buffer = buffer;
        uint32_t next_length;

        //ITERATE OVER MAP
        for(const auto& [key, value] : *thumbs_map) {
            //COPY THUMB LENGTH AND THUMB FILE
            int_buffer = (uint32_t *)buffer;
            next_length = key.size();
            *int_buffer++ = htonl(next_length);
            byte_buffer = (unsigned char *)int_buffer;
            std::copy(key.begin(), key.end(), byte_buffer);
            byte_buffer += next_length;

            //COPY PATH LENGTH AND PATH
            int_buffer = (uint32_t *)byte_buffer;
            next_length = value.length();
            *int_buffer++ = htonl(next_length);
            byte_buffer = (unsigned char *)int_buffer;
            std::copy(key.begin(), key.end(), byte_buffer);
            byte_buffer += next_length;
        }
        int_buffer = (uint32_t *)byte_buffer;
        *int_buffer = htonl(0);
    }
    else { //SERIALIZE BYTE VECTOR
        //GET BYTE VECTOR FROM VARIANT
        std::vector<unsigned char> *byte_vector = std::get<2>(content_);

        //POPULATE BUFFER
        std::copy(byte_vector->begin(), byte_vector->end(), buffer);
    }
}

void payload::deserialize(unsigned char *buffer, uint32_t buffer_size, message_type msg_type) {
    if(msg_type == message_type::UPLOAD_IMAGE) {
        uint32_t *int_buffer;
        unsigned char *byte_buffer;
        uint32_t image_size, category_length;

        //GET IMAGE FILE FROM PAYLOAD
        int_buffer = (uint32_t *)buffer;
        image_size = ntohl(*int_buffer++);
        byte_buffer = (unsigned char *)int_buffer;
        auto image_file = new std::vector<unsigned char>(image_size);
        image_file->assign(byte_buffer, byte_buffer + image_size);

        //GET CATEGORY FROM PAYLOAD
        int_buffer = (uint32_t *)(byte_buffer + image_size);
        category_length = ntohl(*int_buffer++);
        byte_buffer = (unsigned char *)int_buffer;
        auto category = new std::string((char *)byte_buffer, category_length);

        //PUT IMAGE FILE AND CATEGORY IN IMAGE
        content_ = new image(image_file, category);
    }
    else { //VIEW THUMBS OR DOWNLOAD IMAGE
        content_ = new std::string((char *)buffer, buffer_size);
    }
}
