#ifndef SERVER_MESSAGE_H
#define SERVER_MESSAGE_H

#include <cstdint>
#include <map>
#include <ostream>
#include <string>
#include <variant>
#include <vector>
#include "constants.h"
#include "image.h"

class header {
    private:
        MESSAGE_TYPE message_type_;
        uint32_t source_id_;
        uint32_t payload_length_;
    public:
        header();
        MESSAGE_TYPE get_message_type() const;
        uint32_t get_source_id() const;
        uint32_t get_payload_length() const;
        void set_message_type(MESSAGE_TYPE message_type);
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
        void deserialize(unsigned char *buffer, uint32_t buffer_size, unsigned char message_type);
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
