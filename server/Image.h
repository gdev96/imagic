#ifndef SERVER_IMAGE_H
#define SERVER_IMAGE_H

#include <vector>
#include <string>

using namespace std;

class Image {
    private:
        vector<unsigned char *> *image_file, *thumb_file;
        string *image_file_path, *thumb_file_path, *category;
    public:
        Image(vector<unsigned char *> *imageFile, vector<unsigned char *> *thumbFile, string *imageFilePath,
            string *thumbFilePath, string *category);
        void serialize(unsigned char *buffer);
        void deserialize(unsigned char *buffer, uint32_t buffer_length, unsigned char message_type);
};

#endif //SERVER_IMAGE_H
