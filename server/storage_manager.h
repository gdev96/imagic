#ifndef SERVER_STORAGE_MANAGER_H
#define SERVER_STORAGE_MANAGER_H

#include <mysqlx/xdevapi.h>
#include <Magick++.h>
#include <string>
#include "message.h"

enum upload_status : unsigned char {
    UPLOADED = 0,
    DUPLICATE = 1,
    INVALID = 2
};

class storage_manager {
    mysqlx::Session *db_session_;
    mysqlx::Table *current_table_;
    message *current_request_;
    unsigned int server_id_, *last_image_id_;
    bool *last_image_id_read_;
public:
    storage_manager(message *current_request, unsigned int server_id, unsigned int *last_image_id, bool *last_image_id_read);
    ~storage_manager();
    void upload_request();
    void view_thumbs();
    void download_image();
};

#endif //SERVER_STORAGE_MANAGER_H
