#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <mysqlx/xdevapi.h>
#include "image.h"
#include "storage_manager.h"

storage_manager::storage_manager(const message &current_request) : current_request_(current_request) {}

void storage_manager::upload_request() {
    //GET IMAGE AND ITS CONTENT FROM MESSAGE
    image *image = std::get<0>(current_request_.get_payload()->getContent());
    std::vector<unsigned char> *image_file = image->getImageFile();
    std::string *category = image->getCategory();

    //SAVE FILE TO DISK
    std::string path = "image.jpg";
    std::ofstream output_file(path, std::ios::binary);
    output_file.write((const char*)image_file->data(), image_file->size());

    //CREATE A NEW DB SESSION TO ACCESS DATA
    mysqlx::Session session("mysqlx://root@127.0.0.1");

    //OBTAIN SCHEMA/DATABASE FROM SESSION
    mysqlx::Schema schema = session.getSchema("imagic", true);

    //GET TABLE
    mysqlx::Table table = schema.getTable("image", true);

    //CREATE QUERY TO INSERT PATH AND CATEGORY IN DB
    mysqlx::Result result = table.insert("category", "image_path").values(*category, path).execute();
    std::cout << "Image added to database (" << result.getWarningsCount() << " warnings generated)" << std::endl;
}

void storage_manager::view_thumbs() { //VIEW THUMBS -> get thumbs map and send response
    //1.Prelevo i percorsi delle miniature appartenenti ad una categoria
    //2.Prelevo i rispettivi file delle miniature dalla memoria
    //3.Creo la mappa di miniature, la serializzo e la invio.
}

void storage_manager::download_image() { //DOWNLOAD IMAGE -> get the image to send response
    //1.Cerco il path dell'immagine scelta nel db
    //2.Invio come risposta il file immagine scelto prelevato dalla memoria
}
