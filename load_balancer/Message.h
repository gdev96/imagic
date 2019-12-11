#ifndef IMAGIC_BACKEND_MESSAGE_H
#define IMAGIC_BACKEND_MESSAGE_H

#include <cstdint>
#include <ostream>

class Header {
    private:
        uint8_t message_type;
        uint32_t source_id;
        uint32_t payload_length;

    public:
        Header();
        void serialize(unsigned char *buffer);
        void deserialize(const unsigned char *buffer);

    friend std::ostream &operator<<(std::ostream &os, const Header &header);
};

class Message {
    private:
        Header header;
        unsigned char *payload;
    public:
        Message(const Header &header, unsigned char *payload, uint32_t payload_length);
};

#endif //IMAGIC_BACKEND_MESSAGE_H
