#ifndef SERVER_IMAGE_H
#define SERVER_IMAGE_H

#include <string>
#include <vector>

class image {
    std::vector<unsigned char> *file_;
    std::string *category_;
public:
    image(std::vector<unsigned char> *file, std::string *category) : file_(file), category_(category) {}
    ~image();
    std::vector<unsigned char> *get_file() const { return file_; }
    std::string *get_category() const { return category_; }
};

#endif  // SERVER_IMAGE_H
