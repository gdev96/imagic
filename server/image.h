#ifndef SERVER_IMAGE_H
#define SERVER_IMAGE_H

#include <string>
#include <vector>

class image {
    private:
        std::vector<unsigned char> *file_;
        std::string *category_;
    public:
        image(std::vector<unsigned char> *image_file, std::string *category);
        std::vector<unsigned char> *get_file() const;
        std::string *get_category() const;
};

#endif //SERVER_IMAGE_H
