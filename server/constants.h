#ifndef SERVER_CONSTANTS_H
#define SERVER_CONSTANTS_H

#include <string>

//OUTPUT MESSAGE
inline const std::string *OUTPUT_IDENTIFIER;

//CONNECTOR
inline const unsigned int QUEUE_LENGTH_CONNECTIONS = 5;

//MESSAGE
inline const unsigned int CHUNK_SIZE = 8192;

//HEADER
inline const unsigned int HEADER_LENGTH = 9;

#endif //SERVER_CONSTANTS_H
