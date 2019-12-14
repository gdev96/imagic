#include "image.h"

image::image(std::vector<unsigned char> *image_file, std::string *category) : image_file_(image_file),
                                                                              category_(category) {}
