#include "image.h"

image::image(std::vector<unsigned char> *image_file, std::string *category) : image_file_(image_file),
                                                                              category_(category) {}

std::vector<unsigned char> *image::getImageFile() const {
    return image_file_;
}

std::string *image::getCategory() const {
    return category_;
}
