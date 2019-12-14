#ifndef IMAGIC_BACKEND_MESSAGE_H
#define IMAGIC_BACKEND_MESSAGE_H

#include <cstdint>
#include <ostream>
#include <unordered_map>
#include <variant>
#include <vector>

#include "image.h"

using namespace std;

class header {
    private:
        unsigned char message_type_;
        uint32_t source_id_;
        uint32_t payload_length_;
    public:
        header();
        unsigned char get_message_type() const;
        uint32_t get_source_id() const;
        uint32_t get_payload_length() const;
        void set_message_type(unsigned char messageType);
        void set_source_id(uint32_t sourceId);
        void set_payload_length(uint32_t payloadLength);
        void serialize(unsigned char *buffer);
        void deserialize(unsigned char *buffer);
        friend std::ostream &operator<<(std::ostream &os, const header &header);
};

class payload {
    private:
        variant<
                image *,
                string *,
                vector<unsigned char> *,
                unordered_map<vector<unsigned char>,string> *
               > content_;
    public:
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
};

#endif //IMAGIC_BACKEND_MESSAGE_H
