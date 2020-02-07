#include <netinet/in.h>
#include "message.h"

header::header() {}

message_type header::get_message_type() const {
    return message_type_;
}

uint32_t header::get_payload_length() const {
    return payload_length_;
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
    os << "message_type: " << (unsigned int)header.message_type_ << " source_id: " << header.source_id_ << " payload_length: " << header.payload_length_;
    return os;
}

message::message() {}

message::message(header *header, payload *payload) : header_(header), payload_(payload) {}

message::~message() {
    delete header_;
    delete payload_;
}

header *message::get_header() const {
    return header_;
}

payload *message::get_payload() const {
    return payload_;
}

const std::variant<image *, std::string *, std::vector<unsigned char> *, std::map<std::vector<unsigned char>, std::string> *> &payload::get_content() const {
    return content_;
}

void payload::set_content(const std::variant<image *, std::string *, std::vector<unsigned char> *, std::map<std::vector<unsigned char>, std::string> *> &content) {
    content_ = content;
}

void payload::serialize(unsigned char *buffer) {
    switch(content_.index()) {
        case 1: //Serialize string
            //Get string from variant
            std::string *str_message;
            str_message = std::get<1>(content_);
            //Populate buffer
            str_message->copy((char *)buffer, str_message->length());
            break;
        case 2: //Serialize byte vector
            //Get byte vector from variant
            std::vector<unsigned char> *byte_vector;
            byte_vector = std::get<2>(content_);
            //Populate buffer
            std::copy(byte_vector->begin(), byte_vector->end(), buffer);
            break;
        case 3: //Serialize map
            //Get map from variant
            std::map<std::vector<unsigned char>, std::string> *thumbs_map;
            thumbs_map = std::get<3>(content_);

            uint32_t *int_buffer;
            uint32_t next_length;

            //Iterate over map
            for(const auto& [key, value] : *thumbs_map) {
                //Copy thumb file size and thumb file
                int_buffer = (uint32_t *)buffer;
                next_length = key.size();
                *int_buffer++ = htonl(next_length);
                buffer = (unsigned char *)int_buffer;
                std::copy(key.begin(), key.end(), buffer);
                buffer += next_length;

                //Copy path length and path
                int_buffer = (uint32_t *)buffer;
                next_length = value.length();
                *int_buffer++ = htonl(next_length);
                buffer = (unsigned char *)int_buffer;
                std::copy(value.begin(), value.end(), buffer);
                buffer += next_length;
            }
            int_buffer = (uint32_t *)buffer;
            *int_buffer = htonl(0);
    }
}

void payload::deserialize(unsigned char *buffer, uint32_t buffer_size, message_type msg_type) {
    if(msg_type == message_type::UPLOAD_IMAGE) {
        uint32_t *int_buffer;
        unsigned char *byte_buffer;
        uint32_t image_size, category_length;

        //Get image file from payload
        int_buffer = (uint32_t *)buffer;
        image_size = ntohl(*int_buffer++);
        byte_buffer = (unsigned char *)int_buffer;
        auto image_file = new std::vector<unsigned char>(byte_buffer, byte_buffer + image_size);

        //Get category from payload
        int_buffer = (uint32_t *)(byte_buffer + image_size);
        category_length = ntohl(*int_buffer++);
        byte_buffer = (unsigned char *)int_buffer;
        auto category = new std::string((char *)byte_buffer, category_length);

        //Put image file and category in image
        content_ = new image(image_file, category);
    }
    else { //VIEW THUMBS or DOWNLOAD IMAGE
        content_ = new std::string((char *)buffer, buffer_size);
    }
}
