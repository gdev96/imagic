#ifndef SERVER_STORAGE_MANAGER_H
#define SERVER_STORAGE_MANAGER_H

#include <mysqlx/xdevapi.h>
#include <Magick++.h>
#include "message.h"

class storage_manager {
    std::string db_host_, db_user_, db_password_, db_name_, db_table_name_;
    int db_port_;
    std::string images_dir_, thumb_size_;
    bool last_image_id_read_ = false;
    unsigned int last_image_id_;
public:
    storage_manager();
    void upload_image(message *client_message);
    void find_thumbs(message *client_message);
    void download_image(message *client_message);
};

#endif  // SERVER_STORAGE_MANAGER_H
