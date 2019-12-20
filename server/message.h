#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <variant>
#include <vector>
#include "image.h"

enum class message_type : unsigned char {
    UPLOAD_IMAGE = 0,
    FIND_THUMBS = 1,
    DOWNLOAD_IMAGE = 2
};

class header {
    private:
        message_type message_type_;
        uint32_t source_id_;
        uint32_t payload_length_;
    public:
        header();
        message_type get_message_type() const;
        uint32_t get_source_id() const;
        uint32_t get_payload_length() const;
        void set_message_type(message_type msg_type);
        void set_source_id(uint32_t source_id);
        void set_payload_length(uint32_t payload_length);
        void serialize(unsigned char *buffer);
        void deserialize(unsigned char *buffer);
        friend std::ostream &operator<<(std::ostream &os, const header &header);
};

class payload {
    private:
        std::variant<
                image *,
                std::string *,
                std::vector<unsigned char> *,
                std::map<std::vector<unsigned char>, std::string> *
               > content_;
    public:
        const std::variant<image *, std::string *, std::vector<unsigned char> *, std::map<std::vector<unsigned char>, std::string> *> &
        get_content() const;
        void set_content(
            const std::variant<image *, std::string *, std::vector<unsigned char> *,
                    std::map<std::vector<unsigned char>, std::string> *> &content);
        void serialize(unsigned char *buffer);
        void deserialize(unsigned char *buffer, uint32_t buffer_size, message_type msg_type);
};

class message {
    private:
        header *header_;
        payload *payload_;
    public:
        message();
        message(header *header, payload *payload);
        ~message();
        header *get_header() const;
        payload *get_payload() const;
        void set_payload(payload *payload);
};

#endif //SERVER_MESSAGE_H
