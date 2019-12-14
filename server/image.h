#ifndef SERVER_IMAGE_H
#define SERVER_IMAGE_H

#include <string>
#include <vector>

using namespace std;

class image {
    private:
        vector<unsigned char> *image_file_, *thumb_file_;
        string *image_file_path_, *thumb_file_path_, *category_;
    public:
        image(vector<unsigned char> *image_file, string *category);
};

#endif //SERVER_IMAGE_H
