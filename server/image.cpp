#include "image.h"

image::image(std::vector<unsigned char> *file, std::string *category) : file_(file), category_(category) {}

std::vector<unsigned char> *image::get_file() const {
    return file_;
}

std::string *image::get_category() const {
    return category_;
}
