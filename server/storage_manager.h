#ifndef SERVER_STORAGE_MANAGER_H
#define SERVER_STORAGE_MANAGER_H

#include <mysqlx/xdevapi.h>
#include <Magick++.h>
#include "message.h"

class storage_manager {
    mysqlx::Session *db_session_;
    mysqlx::Table *current_table_;
    message *current_request_;
    unsigned int server_id_;
    static bool last_image_id_read_;
    static unsigned int last_image_id_;
public:
    storage_manager(message *current_request, unsigned int server_id);
    ~storage_manager();
    void upload_image();
    void find_thumbs();
    void download_image();
};

#endif  // SERVER_STORAGE_MANAGER_H
