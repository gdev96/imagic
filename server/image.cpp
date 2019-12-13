#include "image.h"
#include "constants.h"

image::image(vector<unsigned char *> *image_file, vector<unsigned char *> *thumb_file, string *image_file_path,
             string *thumb_file_path, string *category) : image_file_(image_file), thumb_file_(thumb_file),
                                                        image_file_path_(image_file_path), thumb_file_path_(thumb_file_path),
                                                        category_(category) {}

void image::serialize(unsigned char *buffer) {

}

void image::deserialize(unsigned char *buffer, uint32_t buffer_length, unsigned char message_type) {
    switch(message_type) {
        case 0: //UPLOAD IMAGE
            uint32_t *int_buffer;
            unsigned char *byte_buffer;
            uint32_t image_length, category_length;

            //GET IMAGE FILE FROM PAYLOAD
            int_buffer = (uint32_t *)buffer;
            image_length = ntohl(*int_buffer++);
            byte_buffer = (unsigned char *)int_buffer;
            image_file_ = new vector<unsigned char *>(image_length);
            image_file_->assign(byte_buffer, byte_buffer + image_length);

            //GET CATEGORY FROM PAYLOAD
            int_buffer = (uint32_t *)(byte_buffer + image_length);
            category_length = ntohl(*int_buffer++);
            byte_buffer = (unsigned char *)int_buffer;
            category_ = new string((char *)byte_buffer, category_length);
            break;
        case 1: //VIEW THUMBS
            category_ = new string((char *)buffer, buffer_length);
            break;
        case 2: //DOWNLOAD IMAGE
            thumb_file_path_ = new string((char *)buffer, buffer_length);
            break;
        default: //ERROR
            break;
    }
}
