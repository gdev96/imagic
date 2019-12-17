#ifndef SERVER_STORAGE_MANAGER_H
#define SERVER_STORAGE_MANAGER_H

#include "message.h"

class storage_manager {
private:
    message current_request_;
    unsigned int server_id_;
public:
    storage_manager(const message &currentRequest, unsigned int serverId);
    void upload_request();
    void view_thumbs();
    void download_image();
};

#endif //SERVER_STORAGE_MANAGER_H
