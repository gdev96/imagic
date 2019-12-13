#include "Image.h"
#include "Constants.h"

Image::Image(vector<unsigned char *> *imageFile, vector<unsigned char *> *thumbFile, string *imageFilePath,
             string *thumbFilePath, string *category) : image_file(imageFile), thumb_file(thumbFile),
                                                        image_file_path(imageFilePath), thumb_file_path(thumbFilePath),
                                                        category(category) {}

void Image::serialize(unsigned char *buffer) {

}

void Image::deserialize(unsigned char *buffer, uint32_t buffer_length, unsigned char message_type) {
    switch(message_type) {
        case 0: //UPLOAD IMAGE
            uint32_t *int_buffer;
            unsigned char *byte_buffer;
            uint32_t image_length, category_length;

            //GET IMAGE FILE FROM PAYLOAD
            int_buffer = (uint32_t *)buffer;
            image_length = ntohl(*int_buffer++);
            byte_buffer = (unsigned char *)int_buffer;
            image_file = new vector<unsigned char *>(image_length);
            image_file->assign(byte_buffer, byte_buffer + image_length);

            //GET CATEGORY FROM PAYLOAD
            int_buffer = (uint32_t *)(byte_buffer + image_length);
            category_length = ntohl(*int_buffer++);
            byte_buffer = (unsigned char *)int_buffer;
            category = new string((char *)byte_buffer, category_length);
            break;
        case 1: //VIEW THUMBS
            category = new string((char *)buffer, buffer_length);
            break;
        case 2: //DOWNLOAD IMAGE
            thumb_file_path = new string((char *)buffer, buffer_length);
            break;
        default: //ERROR
            break;
    }
}
