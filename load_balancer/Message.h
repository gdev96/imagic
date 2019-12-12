#ifndef IMAGIC_BACKEND_MESSAGE_H
#define IMAGIC_BACKEND_MESSAGE_H

#include <cstdint>
#include <ostream>

class Header {
    private:
        unsigned char message_type;
        uint32_t source_id;
        uint32_t payload_length;

    public:
        Header();
        unsigned char getMessageType() const;
        uint32_t getSourceId() const;
        uint32_t getPayloadLength() const;
        void setMessageType(unsigned char messageType);
        void setSourceId(uint32_t sourceId);
        void setPayloadLength(uint32_t payloadLength);
        void serialize(unsigned char *buffer);
        void deserialize(unsigned char *buffer);

    friend std::ostream &operator<<(std::ostream &os, const Header &header);
};

class Message {
    private:
        Header *header;
        unsigned char *payload;
    public:
        Message(Header *header, unsigned char *payload);
        Header *getHeader() const;
        unsigned char *getPayload() const;
};

#endif //IMAGIC_BACKEND_MESSAGE_H
