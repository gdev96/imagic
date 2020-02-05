#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "constants.h"
#include "image.h"
#include "storage_manager.h"

storage_manager::storage_manager(message *current_request, unsigned int server_id) : current_request_(current_request),
                                                                                   server_id_(server_id) {
    std::string db_host(std::getenv("DB_HOST"));
    std::string db_user(std::getenv("DB_USER"));
    std::string db_password(std::getenv("DB_PASSWORD"));
    std::string db_name(std::getenv("DB_NAME"));
    std::string db_table_name(std::getenv("DB_TABLE_NAME"));

    //CREATE A NEW DB SESSION TO ACCESS DATA
    db_session_ = new mysqlx::Session(
            (mysqlx::string)db_host,
            33060,
            (mysqlx::string)db_user,
            (mysqlx::string)db_password
    );

    //OBTAIN SCHEMA/DATABASE FROM SESSION
    mysqlx::Schema schema = db_session_->getSchema((mysqlx::string)(db_name + std::to_string(server_id_)), true);

    //GET TABLE
    current_table_ = new mysqlx::Table(schema.getTable((mysqlx::string)db_table_name, true));
}

storage_manager::~storage_manager() {
    db_session_->close();
    delete db_session_;
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
    std::cout << *OUTPUT_IDENTIFIER << "Image saved in: " + image_file_path << std::endl;

    //SAVE THUMB FILE TO DISK
    std::string thumb_file_path = "server/resources/" + std::to_string(server_id_) + "/image_thumb.jpg";
    std::ofstream output_thumb_file("./" + thumb_file_path, std::ios::binary);
    output_thumb_file.write((const char*)image_file->data(), image_file->size());
    output_thumb_file.close();
    std::cout << *OUTPUT_IDENTIFIER << "Thumb saved in: " + image_file_path << std::endl;

    //CREATE QUERY TO INSERT PATH AND CATEGORY IN DB
    mysqlx::Result result = current_table_
            ->insert("category", "image_file_path", "thumb_file_path")
            .values((mysqlx::string)*category, (mysqlx::string)image_file_path, (mysqlx::string)thumb_file_path)
            .execute();
    std::cout << *OUTPUT_IDENTIFIER << "Image added to database (" << result.getWarningsCount() << " warnings generated)" << std::endl;

    auto response = new std::string("Uploaded");

    //SET PAYLOAD LENGTH IN MESSAGE HEADER
    current_request_->get_header()->set_payload_length(response->length());

    //SET NEW PAYLOAD
    current_request_->get_payload()->set_content(response);
}

void storage_manager::view_thumbs() {

    //GET THUMB_PATH FROM MESSAGE
    std::string *category = std::get<1>(current_request_->get_payload()->get_content());

    //CREATE THUMBS MAP
    auto thumbs_map = new std::map<std::vector<unsigned char>, std::string>;

    //GET THUMB_PATHS FROM DB
    mysqlx::RowResult rows = current_table_
            ->select("thumb_file_path")
            .where("category like :category")
            .bind("category", (mysqlx::string)*category)
            .execute();

    uint32_t payload_length = 4; // Minimum payload length in byte

    for (mysqlx::Row row : rows.fetchAll()) {
        //GET THUMB PATH FROM ROW
        std::string thumb_file_path = (std::string)row[0];

        //GET PATH_FILE FROM DISK
        std::ifstream input_thumb_file("./" + thumb_file_path, std::ios::binary);
        input_thumb_file.seekg(0, std::ifstream::end);
        uint32_t thumb_size = input_thumb_file.tellg();
        input_thumb_file.seekg(0, std::ifstream::beg);

        auto thumb_file = new std::vector<unsigned char>(thumb_size);
        input_thumb_file.read((char *)thumb_file->data(), thumb_size);
        input_thumb_file.close();

        //CREATE THE ENTRY IN THE THUMB'S MAP
        thumbs_map->insert({*thumb_file, thumb_file_path});

        // Update payload length
        payload_length += 8 + thumb_size + thumb_file_path.length();
    }
    //SET PAYLOAD LENGTH IN MESSAGE HEADER
    current_request_->get_header()->set_payload_length(payload_length);

    //SET NEW PAYLOAD
    current_request_->get_payload()->set_content(thumbs_map);
}

void storage_manager::download_image() {
    //GET THUMB_PATH FROM MESSAGE
    std::string *thumb_file_path = std::get<1>(current_request_->get_payload()->get_content());

    //CREATE QUERY TO GET THE IMAGE_PATH FROM THUMB_PATH
    mysqlx::RowResult rows = current_table_
            ->select("image_file_path")
            .where("thumb_file_path like :thumb_file_path")
            .bind("thumb_file_path", *thumb_file_path)
            .execute();
    mysqlx::Row row = rows.fetchOne();
    std::string image_file_path = (std::string)row[0];

    //GET IMAGE_FILE FROM DISK
    std::ifstream input_image_file("./" + image_file_path, std::ios::binary);
    input_image_file.seekg(0, std::ifstream::end);
    uint32_t image_size = input_image_file.tellg();
    input_image_file.seekg(0, std::ifstream::beg);

    auto *image_file = new std::vector<unsigned char>(image_size);
    input_image_file.read((char*)image_file->data(), image_size);
    input_image_file.close();

    //SET PAYLOAD LENGTH IN MESSAGE HEADER
    current_request_->get_header()->set_payload_length(image_size);

    //SET NEW PAYLOAD
    current_request_->get_payload()->set_content(image_file);
}
