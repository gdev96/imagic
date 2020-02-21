#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>

#ifdef TESTING
#include <random>
#include <thread>
#endif

#include <vector>
#include "constants.h"
#include "image.h"
#include "storage_manager.h"

#ifdef TESTING
size_t random_generator(size_t min, size_t max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(min,max);  // Distribution in range [min, max]
    return dist(rng);
}
#endif

bool storage_manager::last_image_id_read_ = false;
unsigned int storage_manager::last_image_id_ = 0;

storage_manager::storage_manager(message *current_request, unsigned int server_id) : current_request_(current_request), server_id_(server_id) {
    // Get database infos
    std::string db_host(std::getenv("DB_HOST"));
    int db_port = std::stoi(std::getenv("DB_PORT"));
    std::string db_user(std::getenv("DB_USER"));
    std::string db_password(std::getenv("DB_PASSWORD"));
    std::string db_name(std::getenv("DB_NAME"));
    std::string db_table_name(std::getenv("DB_TABLE_NAME"));

    // Create a new DB session to access data
    db_session_ = new mysqlx::Session(
            (mysqlx::string)db_host,
            db_port,
            (mysqlx::string)db_user,
            (mysqlx::string)db_password
    );
    // Obtain schema/database from session
    mysqlx::Schema schema = db_session_->getSchema((mysqlx::string)(db_name + "_" + std::to_string(server_id_)), true);

    // Get table
    current_table_ = new mysqlx::Table(schema.getTable((mysqlx::string)db_table_name, true));
}

storage_manager::~storage_manager() {
    db_session_->close();
    delete db_session_;
    delete current_table_;
}

enum upload_status : unsigned char {
    UPLOADED = 0,
    DUPLICATE = 1,
    INVALID = 2
};

void storage_manager::upload_request() {

#ifdef TESTING
    int waiting_time = random_generator(10, 30);
    std::cout << *OUTPUT_IDENTIFIER << "Sleeping for " << waiting_time << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
#endif

    // Get image and its content from message
    image *img = (image *)current_request_->get_payload()->get_content();
    std::vector<unsigned char> *image_file = img->get_file();
    std::string *category = img->get_category();

    // Calculate image hash
    std::string image_hash = std::to_string(std::hash<std::string>{}(std::string(image_file->begin(), image_file->end())));

    // Search for duplicates in DB
    mysqlx::RowResult duplicates = current_table_
            ->select()
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
            // Get last id from DB
            mysqlx::RowResult rows = current_table_
                    ->select("max(id)")
                    .execute();

            mysqlx::Row row = rows.fetchOne();

            if(!row[0].isNull()) {
                last_image_id_ = (unsigned int)row[0];
            }
            last_image_id_read_ = true;
        }

        unsigned int image_id = current_request_->get_header()->get_request_id() + last_image_id_ + 1;

        try {
            // Get thumb and image format
            Magick::Image thumb(Magick::Blob(image_file->data(), image_file->size()));
            std::string image_format = thumb.magick();

            // Convert image format to lowercase letters
            std::transform(image_format.begin(), image_format.end(), image_format.begin(),
                    [](unsigned char c) -> unsigned char { return std::tolower(c); });

            // Replace image format with more popular formats
            if(image_format == "jpeg") {
                image_format = "jpg";
            }
            // Get images dir
            std::string server_images_dir = std::getenv("IMAGES_DIR") + std::to_string(server_id_);

            // Save image file to disk
            std::string image_file_name = std::to_string(image_id) + "." + image_format;
            std::ofstream output_image_file("./" + server_images_dir + "/" + image_file_name, std::ios::binary);
            output_image_file.write((const char *) image_file->data(), image_file->size());
            output_image_file.close();
            std::cout << *OUTPUT_IDENTIFIER << "Image \"" + image_file_name << "\" saved" << std::endl;

            // Generate thumb
            thumb.resize(std::getenv("THUMB_SIZE"));

            // Save thumb file to disk
            std::string thumb_file_name = std::to_string(image_id) + "_thumb." + image_format;
            thumb.write("./" + server_images_dir + "/" + thumb_file_name);
            std::cout << *OUTPUT_IDENTIFIER << "Thumb \"" + thumb_file_name << "\" saved" << std::endl;

            // Create query to insert image infos in DB
            mysqlx::Result result = current_table_
                    ->insert("id", "hash", "file_name", "thumb_file_name", "category")
                    .values(image_id, (mysqlx::string) image_hash, (mysqlx::string) image_file_name, (mysqlx::string) thumb_file_name, (mysqlx::string) *category)
                    .execute();
            std::cout << *OUTPUT_IDENTIFIER << "Image added to database" << std::endl;

            response = UPLOADED;
        } catch(const Magick::ErrorMissingDelegate &e) {
            // No valid image format found
            response = INVALID;
        }
    }
    // Set payload length in message header
    current_request_->get_header()->set_payload_length(1);

    // Delete old payload
    delete current_request_->get_payload();

    // Set new payload
    current_request_->set_payload(new byte_payload(new std::vector<unsigned char> {response}));

#ifdef TESTING
    std::cout << *OUTPUT_IDENTIFIER << "Image upload finished" << std::endl;
#endif

}

void storage_manager::view_thumbs() {

#ifdef TESTING
    int waiting_time = random_generator(10, 30);
    std::cout << *OUTPUT_IDENTIFIER << "Sleeping for " << waiting_time << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
#endif

    // Get category from message
    std::string *category = (std::string *)current_request_->get_payload()->get_content();

    // Create thumbs map
    auto thumbs_map = new std::map<std::vector<unsigned char>, std::string>;

    // Get thumb file names from DB
    mysqlx::RowResult rows = current_table_
            ->select("thumb_file_name")
            .where("category like :category")
            .bind("category", (mysqlx::string)*category)
            .execute();

    uint32_t payload_length = 4;  // Minimum payload length in byte (when length = 0)

    for (mysqlx::Row row : rows.fetchAll()) {
        // Get thumb file name from row
        std::string thumb_file_name = (std::string)row[0];

        // Get thumb file path
        std::string thumb_file_path = std::getenv("IMAGES_DIR") + std::to_string(server_id_) + "/" + thumb_file_name;

        // Get thumb file from disk
        std::ifstream input_thumb_file("./" + thumb_file_path, std::ios::binary);

        input_thumb_file.seekg(0, std::ifstream::end);
        uint32_t thumb_size = input_thumb_file.tellg();
        input_thumb_file.seekg(0, std::ifstream::beg);

        auto thumb_file = std::vector<unsigned char>(thumb_size);
        input_thumb_file.read((char *)thumb_file.data(), thumb_size);
        input_thumb_file.close();

        // Create an entry in thumbs map
        thumbs_map->insert({thumb_file, thumb_file_name});

        // Update payload length
        payload_length += 8 + thumb_size + thumb_file_name.length();
    }
    // Set payload length in message header
    current_request_->get_header()->set_payload_length(payload_length);

    // Delete old payload
    delete current_request_->get_payload();

    // Set new payload
    current_request_->set_payload(new thumbs_payload(thumbs_map));

#ifdef TESTING
    std::cout << *OUTPUT_IDENTIFIER << "Image thumbs retrieval finished" << std::endl;
#endif

}

void storage_manager::download_image() {

#ifdef TESTING
    int waiting_time = random_generator(10, 30);
    std::cout << *OUTPUT_IDENTIFIER << "Sleeping for " << waiting_time << " seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(waiting_time));
#endif

    // Get thumb file name from message
    std::string *thumb_file_name = (std::string *)current_request_->get_payload()->get_content();

    // Create query to get the image file name from thumb file name
    mysqlx::RowResult rows = current_table_
            ->select("file_name")
            .where("thumb_file_name like :thumb_file_name")
            .bind("thumb_file_name", *thumb_file_name)
            .execute();

    mysqlx::Row row = rows.fetchOne();
    std::string image_file_name = (std::string)row[0];

    // Get image file path
    std::string image_file_path = std::getenv("IMAGES_DIR") + std::to_string(server_id_) + "/" + image_file_name;

    // Get image file from disk
    std::ifstream input_image_file("./" + image_file_path, std::ios::binary);

    input_image_file.seekg(0, std::ifstream::end);
    uint32_t image_size = input_image_file.tellg();
    input_image_file.seekg(0, std::ifstream::beg);

    auto *image_file = new std::vector<unsigned char>(image_size);
    input_image_file.read((char *)image_file->data(), image_size);
    input_image_file.close();

    // Set payload length in message header
    current_request_->get_header()->set_payload_length(image_size);

    // Delete old payload
    delete current_request_->get_payload();

    // Set new payload
    current_request_->set_payload(new byte_payload(image_file));

#ifdef TESTING
    std::cout << *OUTPUT_IDENTIFIER << "Image download finished" << std::endl;
#endif

}
