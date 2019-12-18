#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "image.h"
#include "storage_manager.h"

storage_manager::storage_manager(message *current_request, unsigned int server_id) : current_request_(current_request),
                                                                                   server_id_(server_id) {}

storage_manager::~storage_manager() {
    db_session_->close();
    delete db_session_;
}

mysqlx::Table storage_manager::connect(const std::string &url, const std::string &database, const std::string &table) {
    //CREATE A NEW DB SESSION TO ACCESS DATA
    db_session_ = new mysqlx::Session(url);

    //OBTAIN SCHEMA/DATABASE FROM SESSION
    mysqlx::Schema schema = db_session_->getSchema(database, true);

    //RETURN TABLE
    return schema.getTable(table, true);
}

void storage_manager::upload_request() {
    //GET IMAGE AND ITS CONTENT FROM MESSAGE
    image *image = std::get<0>(current_request_->get_payload()->get_content());
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

    //CONNECT TO DB
    mysqlx::Table image_table = connect("mysqlx://root@127.0.0.1", "imagic", "image");

    //CREATE QUERY TO INSERT PATH AND CATEGORY IN DB
    mysqlx::Result result = image_table.insert("category", "image_path", "thumb_path").values(*category, image_path, thumb_path).execute();
    std::cout << "Image added to database (" << result.getWarningsCount() << " warnings generated)" << std::endl;

    auto response = new std::string("Uploaded");

    //SET PAYLOAD LENGTH IN MESSAGE HEADER
    current_request_->get_header()->set_payload_length(response->length());

    //SET NEW PAYLOAD
    current_request_->get_payload()->set_content(response);
}

void storage_manager::view_thumbs() { //VIEW THUMBS -> get thumbs map and send response
    //1.Prelevo i percorsi delle miniature appartenenti ad una categoria
    //2.Prelevo i rispettivi file delle miniature dalla memoria
    //3.Creo la mappa di miniature, la serializzo e la invio.

    //GET THUMB_PATH FROM MESSAGE
    std::string *category = std::get<std::string *>(current_request_->get_payload()->get_content());

    //CONNECT TO DB
    mysqlx::Table image_table = connect("mysqlx://root@127.0.0.1", "imagic", "image");

    //CREATE THUMBS MAP
    std::map<std::vector<unsigned char>, std::string> *thumbs_map;

    //GET THUMB_PATHS FROM DB
    mysqlx::RowResult rows = image_table.select("thumb_path").where("category like: category")
            .bind("category", *category).execute();
    for (mysqlx::Row row : rows.fetchAll())
    {
        //GET THUMB PATH FROM ROW
        mysqlx::string thumb_path = row[0];

        //GET PATH_FILE FROM DISK
        auto *thumb_file = new std::vector<unsigned char>;
        std::ifstream input_thumb_file(thumb_path, std::ios::binary);
        input_thumb_file.seekg(0, std::ifstream::end);
        uint32_t image_size = input_thumb_file.tellg();
        input_thumb_file.seekg(0, std::ifstream::beg);
        input_thumb_file.read((char*)thumb_file->data(), image_size);
        input_thumb_file.close();

        //CREATE THE ENTRY IN THE THUMB'S MAP
        thumbs_map->insert({*thumb_file,thumb_path});

        //SET PAYLOAD LENGTH IN MESSAGE HEADER
        current_request_->get_header()->set_payload_length(thumbs_map->size());

        //SET NEW PAYLOAD
        current_request_->get_payload()->set_content(thumbs_map);
    }
}

void storage_manager::download_image() {
    //GET THUMB_PATH FROM MESSAGE
    std::string *thumb_path = std::get<std::string *>(current_request_->get_payload()->get_content());

    //CONNECT TO DB
    mysqlx::Table image_table = connect("mysqlx://root@127.0.0.1", "imagic", "image");

    //CREATE QUERY TO GET THE IMAGE_PATH FROM THUMB_PATH
    mysqlx::RowResult rows = image_table.select("image_path").where("thumb_path like: thumb_path")
            .bind("thumb_path", *thumb_path).execute();
    mysqlx::Row row = rows.fetchOne();
    mysqlx::string image_path = row[0];

    //GET IMAGE_FILE FROM DISK
    auto *image_file = new std::vector<unsigned char>;
    std::ifstream input_image_file(image_path, std::ios::binary);
    input_image_file.seekg(0, std::ifstream::end);
    uint32_t image_size = input_image_file.tellg();
    input_image_file.seekg(0, std::ifstream::beg);
    input_image_file.read((char*)image_file->data(), image_size);
    input_image_file.close();

    //SET PAYLOAD LENGTH IN MESSAGE HEADER
    current_request_->get_header()->set_payload_length(image_size);

    //SET NEW PAYLOAD
    current_request_->get_payload()->set_content(image_file);
}
