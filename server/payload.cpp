#include <netinet/in.h>
#include "payload.h"

string_payload::~string_payload() {
    delete content_;
}

void string_payload::deserialize(unsigned char *buffer, uint32_t buffer_size) {
    // Get string from buffer
    content_ = new std::string((char *)buffer, buffer_size);
}

byte_payload::~byte_payload() {
    delete content_;
}

void byte_payload::serialize(unsigned char *buffer) {
    // Populate buffer
    std::copy(content_->begin(), content_->end(), buffer);
}

image_payload::~image_payload() {
    delete content_;
}

void image_payload::deserialize(unsigned char *buffer, uint32_t buffer_size) {
    uint32_t *int_buffer;
    unsigned char *byte_buffer;
    uint32_t image_size, category_length;

    // Get image file from payload
    int_buffer = (uint32_t *)buffer;
    image_size = ntohl(*int_buffer++);
    byte_buffer = (unsigned char *)int_buffer;
    auto image_file = new std::vector<unsigned char>(byte_buffer, byte_buffer + image_size);

    // Get category from payload
    int_buffer = (uint32_t *)(byte_buffer + image_size);
    category_length = ntohl(*int_buffer++);
    byte_buffer = (unsigned char *)int_buffer;
    auto category = new std::string((char *)byte_buffer, category_length);

    // Put image file and category in image
    content_ = new image(image_file, category);
}

thumbs_payload::~thumbs_payload() {
    delete content_;
}

void thumbs_payload::serialize(unsigned char *buffer) {
    uint32_t *int_buffer;
    uint32_t next_length;

    // Iterate over thumbs vector
    for(const auto &[thumb_file, thumb_file_name] : *content_) {
        // Copy thumb file size and thumb file
        int_buffer = (uint32_t *)buffer;
        next_length = thumb_file.size();
        *int_buffer++ = htonl(next_length);
        buffer = (unsigned char *)int_buffer;
        std::copy(thumb_file.begin(), thumb_file.end(), buffer);
        buffer += next_length;

        // Copy thumb file name length and thumb file name
        int_buffer = (uint32_t *)buffer;
        next_length = thumb_file_name.length();
        *int_buffer++ = htonl(next_length);
        buffer = (unsigned char *)int_buffer;
        std::copy(thumb_file_name.begin(), thumb_file_name.end(), buffer);
        buffer += next_length;
    }
    int_buffer = (uint32_t *)buffer;
    *int_buffer = htonl(0);
}
