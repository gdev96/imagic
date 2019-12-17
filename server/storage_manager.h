#ifndef SERVER_STORAGE_MANAGER_H
#define SERVER_STORAGE_MANAGER_H

#include <mysqlx/xdevapi.h>
#include "message.h"

class storage_manager {
private:
    message current_request_;
    unsigned int server_id_;
public:
    static mysqlx::Table connect(const std::string &url, const std::string &database, const std::string &table);
    storage_manager(const message &current_request, unsigned int server_id);
    void upload_request();
    void view_thumbs();
    void download_image();
};

#endif //SERVER_STORAGE_MANAGER_H
