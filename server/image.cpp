#include "image.h"

image::~image() {
    delete file_;
    delete category_;
}
