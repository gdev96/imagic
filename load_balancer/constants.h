#ifndef IMAGIC_BACKEND_CONSTANTS_H
#define IMAGIC_BACKEND_CONSTANTS_H

#include <string>

//MACROS
inline const std::string *OUTPUT_IDENTIFIER;

//CONNECTORS
inline const unsigned int QUEUE_LENGTH_CONNECTIONS = 5;

//MESSAGE
inline const unsigned int CHUNK_SIZE = 8192;

//HEADER
inline const unsigned int HEADER_LENGTH = 9;

#endif //IMAGIC_BACKEND_CONSTANTS_H
