#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "constants.h"
#include "image.h"
#include "storage_manager.h"

storage_manager::storage_manager(message *current_request, unsigned int server_id) : current_request_(current_request),
                                                                                   server_id_(server_id) {}

storage_manager::~storage_manager() {
    db_session_->close();
    delete db_session_;
}

mysqlx::Table storage_manager::connect(const std::string &connection, const std::string &database, const std::string &table) {
    //CREATE A NEW DB SESSION TO ACCESS DATA
    db_session_ = new mysqlx::Session(connection);

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
    std::string image_file_path = "server/resources/" + std::to_string(server_id_) + "/image.jpg";
    std::ofstream output_image_file("./" + image_file_path, std::ios::binary);
    output_image_file.write((const char*)image_file->data(), image_file->size());
    output_image_file.close();
    std::cout << SERVER << "Image saved in: " + image_file_path << std::endl;

    //SAVE THUMB FILE TO DISK
    std::string thumb_file_path = "server/resources/" + std::to_string(server_id_) + "/image_thumb.jpg";
    std::ofstream output_thumb_file("./" + thumb_file_path, std::ios::binary);
    output_thumb_file.write((const char*)image_file->data(), image_file->size());
    output_thumb_file.close();
    std::cout << SERVER << "Thumb saved in: " + image_file_path << std::endl;

    //CONNECT TO DB
    mysqlx::Table image_table = connect("mysqlx://imagicuser:ImgApp2020!@127.0.0.1", "imagic", "image");

    //CREATE QUERY TO INSERT PATH AND CATEGORY IN DB
    mysqlx::Result result = image_table
            .insert("category", "image_file_path", "thumb_file_path")
            .values((mysqlx::string)*category, (mysqlx::string)image_file_path, (mysqlx::string)thumb_file_path)
            .execute();
    std::cout << SERVER << "Image added to database (" << result.getWarningsCount() << " warnings generated)" << std::endl;

    auto response = new std::string("Uploaded");

    //SET PAYLOAD LENGTH IN MESSAGE HEADER
    current_request_->get_header()->set_payload_length(response->length());

    //SET NEW PAYLOAD
    current_request_->get_payload()->set_content(response);
}

void storage_manager::view_thumbs() {

    //GET THUMB_PATH FROM MESSAGE
    std::string *category = std::get<1>(current_request_->get_payload()->get_content());

    //CONNECT TO DB
    mysqlx::Table image_table = connect("mysqlx://imagicuser:ImgApp2020!@127.0.0.1", "imagic", "image");

    //CREATE THUMBS MAP
    auto thumbs_map = new std::map<std::vector<unsigned char>, std::string>;

    //GET THUMB_PATHS FROM DB
    mysqlx::RowResult rows = image_table
            .select("thumb_file_path")
            .where("category like :category")
            .bind("category", (mysqlx::string)*category)
            .execute();
    for (mysqlx::Row row : rows.fetchAll()) {
        //GET THUMB PATH FROM ROW
        mysqlx::string thumb_file_path = row[0];

        //GET PATH_FILE FROM DISK
        auto thumb_file = new std::vector<unsigned char>;
        std::ifstream input_thumb_file("./" + (std::string)thumb_file_path, std::ios::binary);
        input_thumb_file.seekg(0, std::ifstream::end);
        uint32_t thumb_size = input_thumb_file.tellg();
        input_thumb_file.seekg(0, std::ifstream::beg);
        input_thumb_file.read((char*)thumb_file->data(), thumb_size);
        input_thumb_file.close();

        //CREATE THE ENTRY IN THE THUMB'S MAP
        thumbs_map->insert({*thumb_file,thumb_file_path});

        //SET PAYLOAD LENGTH IN MESSAGE HEADER
        current_request_->get_header()->set_payload_length(thumbs_map->size());

        //SET NEW PAYLOAD
        current_request_->get_payload()->set_content(thumbs_map);
    }
}

void storage_manager::download_image() {
    //GET THUMB_PATH FROM MESSAGE
    std::string *thumb_file_path = std::get<1>(current_request_->get_payload()->get_content());

    //CONNECT TO DB
    mysqlx::Table image_table = connect("mysqlx://imagicuser:ImgApp2020!@127.0.0.1", "imagic", "image");

    //CREATE QUERY TO GET THE IMAGE_PATH FROM THUMB_PATH
    mysqlx::RowResult rows = image_table
            .select("image_file_path")
            .where("thumb_file_path like :thumb_file_path")
            .bind("thumb_file_path", *thumb_file_path)
            .execute();
    mysqlx::Row row = rows.fetchOne();
    mysqlx::string image_file_path = row[0];

    //GET IMAGE_FILE FROM DISK
    auto *image_file = new std::vector<unsigned char>;
    std::ifstream input_image_file("./" + (std::string)image_file_path, std::ios::binary);
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
