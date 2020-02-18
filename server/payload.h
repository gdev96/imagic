#ifndef SERVER_PAYLOAD_H
#define SERVER_PAYLOAD_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "image.h"

class payload {
public:
    virtual ~payload() {}
    virtual void *get_content() const = 0;
    virtual void serialize(unsigned char *buffer) = 0;
    virtual void deserialize(unsigned char *buffer, uint32_t buffer_size) = 0;
};

class string_payload : public payload {
    std::string *content_;
public:
    ~string_payload() override;
    void *get_content() const override { return content_; }
    void serialize(unsigned char *buffer) override {}
    void deserialize(unsigned char *buffer, uint32_t buffer_size) override;
};

class byte_payload : public payload {
    std::vector<unsigned char> *content_;
public:
    byte_payload(std::vector<unsigned char> *content) : content_(content) {}
    ~byte_payload() override;
    void *get_content() const override { return content_; }
    void serialize(unsigned char *buffer) override;
    void deserialize(unsigned char *buffer, uint32_t buffer_size) override {}
};

class image_payload : public payload {
    image *content_;
public:
    ~image_payload() override;
    void *get_content() const override { return content_; }
    void serialize(unsigned char *buffer) override {}
    void deserialize(unsigned char *buffer, uint32_t buffer_size) override;
};

class thumbs_payload : public payload {
    std::map<std::vector<unsigned char>, std::string> *content_;
public:
    thumbs_payload(std::map<std::vector<unsigned char>, std::string> *content) : content_(content) {}
    ~thumbs_payload() override;
    void *get_content() const override { return content_; }
    void serialize(unsigned char *buffer) override;
    void deserialize(unsigned char *buffer, uint32_t buffer_size) override {}
};

#endif //SERVER_PAYLOAD_H
