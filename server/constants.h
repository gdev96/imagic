#ifndef SERVER_CONSTANTS_H
#define SERVER_CONSTANTS_H

#include <string>

//Output string
inline const std::string *OUTPUT_IDENTIFIER;

//Connector
inline const unsigned int QUEUE_LENGTH_CONNECTIONS = 5;

//Message
inline const unsigned int CHUNK_SIZE = 8192;

//Header
inline const unsigned int HEADER_LENGTH = 9;

#endif //SERVER_CONSTANTS_H
