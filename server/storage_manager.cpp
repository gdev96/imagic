#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <mysqlx/xdevapi.h>
#include "image.h"
#include "storage_manager.h"


storage_manager::storage_manager(const message &currentRequest, unsigned int serverId) : current_request_(
        currentRequest), server_id_(serverId) {}

void storage_manager::upload_request() {
    //GET IMAGE AND ITS CONTENT FROM MESSAGE
    image *image = std::get<0>(current_request_.get_payload()->getContent());
    std::vector<unsigned char> *image_file = image->getImageFile();
    std::string *category = image->getCategory();

    //SAVE IMAGE FILE TO DISK
    std::string image_path = "resources/" + std::to_string(server_id_) + "/image.jpg";
    std::ofstream output_image_file(image_path, std::ios::binary);
    output_image_file.write((const char*)image_file->data(), image_file->size());
    output_image_file.close();

    //SAVE THUMB FILE TO DISK
    std::string thumb_path = "resources/" + std::to_string(server_id_) + "/image_thumb.jpg";
    std::ofstream output_thumb_file(thumb_path, std::ios::binary);
    output_thumb_file.write((const char*)image_file->data(), image_file->size());
    output_thumb_file.close();

    //CREATE A NEW DB SESSION TO ACCESS DATA
    mysqlx::Session session("mysqlx://root@127.0.0.1");

    //OBTAIN SCHEMA/DATABASE FROM SESSION
    mysqlx::Schema schema = session.getSchema("imagic", true);

    //GET TABLE
    mysqlx::Table table = schema.getTable("image", true);

    //CREATE QUERY TO INSERT PATH AND CATEGORY IN DB
    mysqlx::Result result = table.insert("category", "image_path", "thumb_path").values(*category, image_path, thumb_path).execute();
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

    //GET THUMB_PATH FROM MESSAGE
    std::string *thumb_path = std::get<1>(current_request_.get_payload()->getContent());

    //CREATE A NEW DB SESSION TO ACCESS DATA
    mysqlx::Session session("mysqlx://root@127.0.0.1");

    //OBTAIN SCHEMA/DATABASE FROM SESSION
    mysqlx::Schema schema = session.getSchema("imagic", true);

    //GET TABLE
    mysqlx::Table table = schema.getTable("image", true);

    //CREATE QUERY TO GET THE IMAGE_PATH FROM THUMB_PATH
    mysqlx::RowResult rows = table.select("image_path").where("thumb_path like: thumb_path")
            .bind("thumb_path", *thumb_path).execute();
    mysqlx::Row row = rows.fetchOne();
    mysqlx::string image_path = row[0];

    //GET IMAGE_FILE FROM DISK
    auto *image_file = new std::vector<unsigned char>;

    std::ifstream input_image_file(image_path, std::ios::binary);
    input_image_file.seekg(0, std::ifstream::end);
    int length = input_image_file.tellg();
    input_image_file.seekg(0, std::ifstream::beg);
    input_image_file.read((char*)image_file->data(), length);
    input_image_file.close();

    current_request_.set_payload(image_file);
    //CREARE FUNZIONE PER LA CONNESSIONE AL DB
    //AGGIUNGERE ENUM MESSAGE TYPE CON TYPEDEF
}
