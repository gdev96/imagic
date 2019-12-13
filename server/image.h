#ifndef SERVER_IMAGE_H
#define SERVER_IMAGE_H

#include <string>
#include <vector>

using namespace std;

class image {
    private:
        vector<unsigned char *> *image_file_, *thumb_file_;
        string *image_file_path_, *thumb_file_path_, *category_;
    public:
        image(vector<unsigned char *> *image_file, vector<unsigned char *> *thumb_file, string *image_file_path,
              string *thumb_file_path, string *category);
        void serialize(unsigned char *buffer);
        void deserialize(unsigned char *buffer, uint32_t buffer_length, unsigned char message_type);
};

#endif //SERVER_IMAGE_H
