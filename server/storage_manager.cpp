#include <cstdlib>
#include <fstream>
#include <iostream>

#ifdef TESTING
#include <random>
#include <thread>
#endif

#include <utility>
#include <vector>
#include "constants.h"
#include "image.h"
#include "storage_manager.h"

storage_manager::storage_manager() {
    // Get database infos
    db_host_ = std::string(std::getenv("DB_HOST"));
    db_port_ = std::stoi(std::getenv("DB_PORT"));
    db_user_ = std::string(std::getenv("DB_USER"));
    db_password_ = std::string(std::getenv("DB_PASSWORD"));
    db_name_ = std::string(std::getenv("DB_NAME")) + "_" + std::string(std::getenv("SERVER_ID"));
    db_table_name_ = std::string(std::getenv("DB_TABLE_NAME"));

    // Get images directory
    images_dir_ = std::string(std::getenv("IMAGES_DIR")) + std::string(std::getenv("SERVER_ID"));
}

#ifdef TESTING
size_t random_generator(size_t min, size_t max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min,max);  // Distribution in range [min, max]
    return dist(rng);
}
#endif

enum upload_status : unsigned char {
    UPLOADED = 0,
    DUPLICATE = 1,
    INVALID = 2
};

void storage_manager::upload_image(message *client_message) {

#ifdef TESTING
    int waiting_time = random_generator(10, 30);
    std::cout << *OUTPUT_IDENTIFIER << "Sleeping for " << waiting_time << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
#endif

    // Get image and its content from message
    image *img = (image *)client_message->get_payload()->get_content();
    std::vector<unsigned char> *image_file = img->get_file();
    std::string *category = img->get_category();

    // Calculate image hash
    std::string image_hash = std::to_string(std::hash<std::string>{}(std::string(image_file->begin(), image_file->end())));

    // Create a new DB session to access data
    mysqlx::Session db_session = mysqlx::Session(
            (mysqlx::string)db_host_,
            db_port_,
            (mysqlx::string)db_user_,
            (mysqlx::string)db_password_
    );
    // Get table from schema/database in session
    mysqlx::Table db_table = db_session
            .getSchema((mysqlx::string)db_name_, true)
            .getTable((mysqlx::string)db_table_name_, true);

    // Search for duplicates in DB
    mysqlx::RowResult duplicates = db_table
            .select()
            .where("category like :category AND hash like :hash")
            .bind("category", (mysqlx::string)*category)
            .bind("hash", (mysqlx::string)image_hash)
            .execute();

    unsigned char response;

    if(duplicates.count() > 0) {
        response = DUPLICATE;
    }
    else {
        if(!last_image_id_read_) {
            // Get last image id stored in DB
            mysqlx::RowResult rows = db_table
                    .select("MAX(id)")
                    .execute();

            mysqlx::Row row = rows.fetchOne();

            last_image_id_ = row[0].isNull() ? 0 : (unsigned int)row[0];
            last_image_id_read_ = true;
        }

        unsigned int image_id = client_message->get_header()->get_request_id() + last_image_id_ + 1;

        try {
            // Get thumb and image format
            Magick::Image thumb(Magick::Blob(image_file->data(), image_file->size()));
            std::string image_format = thumb.magick();

            // Convert image format to lowercase letters
            std::transform(image_format.begin(), image_format.end(), image_format.begin(),
                    [](unsigned char c) -> unsigned char { return std::tolower(c); });

            // Replace image format with more popular extensions
            if(image_format == "jpeg") {
                image_format = "jpg";
            }
            else if(image_format == "tiff") {
                image_format = "tif";
            }

            // Save image file to disk
            std::string image_file_name = std::to_string(image_id) + "." + image_format;
            std::ofstream output_image_file("./" + images_dir_ + "/" + image_file_name, std::ios::binary);
            output_image_file.write((const char *)image_file->data(), image_file->size());
            output_image_file.close();
            std::cout << *OUTPUT_IDENTIFIER << "Image \"" + image_file_name << "\" saved" << std::endl;

            // Generate thumb
            thumb.resize(THUMB_SIZE);

            // Save thumb file to disk
            std::string thumb_file_name = std::to_string(image_id) + "_thumb." + image_format;
            thumb.write("./" + images_dir_ + "/" + thumb_file_name);
            std::cout << *OUTPUT_IDENTIFIER << "Thumb \"" + thumb_file_name << "\" saved" << std::endl;

            // Create query to insert image infos in DB
            mysqlx::Result result = db_table
                    .insert("id", "hash", "file_name", "thumb_file_name", "category")
                    .values(image_id, (mysqlx::string) image_hash, (mysqlx::string) image_file_name, (mysqlx::string) thumb_file_name, (mysqlx::string) *category)
                    .execute();
            std::cout << *OUTPUT_IDENTIFIER << "Image added to database" << std::endl;

            response = UPLOADED;
        } catch(const Magick::ErrorMissingDelegate &e) {
            // No valid image format found
            response = INVALID;
        }
    }
    // Close DB connection
    db_session.close();

    // Set payload length in message header
    client_message->get_header()->set_payload_length(1);

    // Delete old payload
    delete client_message->get_payload();

    // Set new payload
    client_message->set_payload(new byte_payload(new std::vector<unsigned char> {response}));

#ifdef TESTING
    std::cout << *OUTPUT_IDENTIFIER << "Image upload finished" << std::endl;
#endif

}

void storage_manager::find_thumbs(message *client_message) {

#ifdef TESTING
    int waiting_time = random_generator(10, 30);
    std::cout << *OUTPUT_IDENTIFIER << "Sleeping for " << waiting_time << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
#endif

    // Get category from message
    std::string *category = (std::string *)client_message->get_payload()->get_content();

    // Create thumbs vector
    auto thumbs_vector = new std::vector<std::pair<std::vector<unsigned char>, std::string>>;

    // Create a new DB session to access data
    mysqlx::Session db_session = mysqlx::Session(
            (mysqlx::string)db_host_,
            db_port_,
            (mysqlx::string)db_user_,
            (mysqlx::string)db_password_
    );
    // Get table from schema/database in session
    mysqlx::Table db_table = db_session
            .getSchema((mysqlx::string)db_name_, true)
            .getTable((mysqlx::string)db_table_name_, true);

    // Get thumb file names from DB
    mysqlx::RowResult rows = db_table
            .select("thumb_file_name")
            .where("category like :category")
            .orderBy("id DESC")
            .bind("category", (mysqlx::string)*category)
            .execute();

    // Close DB connection
    db_session.close();

    uint32_t payload_length = 4;  // Minimum payload length in byte (when length = 0)

    for(mysqlx::Row row : rows.fetchAll()) {
        // Get thumb file name from row
        std::string thumb_file_name = (std::string)row[0];

        // Get thumb file path
        std::string thumb_file_path = images_dir_ + "/" + thumb_file_name;

        // Get thumb file from disk
        std::ifstream input_thumb_file("./" + thumb_file_path, std::ios::binary);

        input_thumb_file.seekg(0, std::ifstream::end);
        uint32_t thumb_size = input_thumb_file.tellg();
        input_thumb_file.seekg(0, std::ifstream::beg);

        auto thumb_file = std::vector<unsigned char>(thumb_size);
        input_thumb_file.read((char *)thumb_file.data(), thumb_size);
        input_thumb_file.close();

        // Create an entry in thumbs vector
        thumbs_vector->push_back(std::make_pair(thumb_file, thumb_file_name));

        // Update payload length
        payload_length += 8 + thumb_size + thumb_file_name.length();
    }
    // Set payload length in message header
    client_message->get_header()->set_payload_length(payload_length);

    // Delete old payload
    delete client_message->get_payload();

    // Set new payload
    client_message->set_payload(new thumbs_payload(thumbs_vector));

#ifdef TESTING
    std::cout << *OUTPUT_IDENTIFIER << "Image thumbs retrieval finished" << std::endl;
#endif

}

void storage_manager::download_image(message *client_message) {

#ifdef TESTING
    int waiting_time = random_generator(10, 30);
    std::cout << *OUTPUT_IDENTIFIER << "Sleeping for " << waiting_time << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
#endif

    // Get thumb file name from message
    std::string *thumb_file_name = (std::string *)client_message->get_payload()->get_content();

    // Create a new DB session to access data
    mysqlx::Session db_session = mysqlx::Session(
            (mysqlx::string)db_host_,
            db_port_,
            (mysqlx::string)db_user_,
            (mysqlx::string)db_password_
    );
    // Get table from schema/database in session
    mysqlx::Table db_table = db_session
            .getSchema((mysqlx::string)db_name_, true)
            .getTable((mysqlx::string)db_table_name_, true);

    // Create query to get the image file name from thumb file name
    mysqlx::RowResult rows = db_table
            .select("file_name")
            .where("thumb_file_name like :thumb_file_name")
            .bind("thumb_file_name", *thumb_file_name)
            .execute();

    // Close DB connection
    db_session.close();

    mysqlx::Row row = rows.fetchOne();
    std::string image_file_name = (std::string)row[0];

    // Get image file path
    std::string image_file_path = images_dir_ + "/" + image_file_name;

    // Get image file from disk
    std::ifstream input_image_file("./" + image_file_path, std::ios::binary);

    input_image_file.seekg(0, std::ifstream::end);
    uint32_t image_size = input_image_file.tellg();
    input_image_file.seekg(0, std::ifstream::beg);

    auto image_file = new std::vector<unsigned char>(image_size);
    input_image_file.read((char *)image_file->data(), image_size);
    input_image_file.close();

    // Set payload length in message header
    client_message->get_header()->set_payload_length(image_size);

    // Delete old payload
    delete client_message->get_payload();

    // Set new payload
    client_message->set_payload(new byte_payload(image_file));

#ifdef TESTING
    std::cout << *OUTPUT_IDENTIFIER << "Image download finished" << std::endl;
#endif

}
