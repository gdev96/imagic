#ifndef SERVER_IMAGE_H
#define SERVER_IMAGE_H

#include <string>
#include <vector>

class image {
    private:
        std::vector<unsigned char> *image_file_, *thumb_file_;
        std::string *image_file_path_, *thumb_file_path_, *category_;
    public:
        image(std::vector<unsigned char> *image_file, std::string *category);
        std::vector<unsigned char> *getImageFile() const;
        std::string *getCategory() const;
};

#endif //SERVER_IMAGE_H
