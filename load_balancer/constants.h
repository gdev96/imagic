#ifndef IMAGIC_BACKEND_CONSTANTS_H
#define IMAGIC_BACKEND_CONSTANTS_H

#include <string>

//Output string
inline const std::string *OUTPUT_IDENTIFIER;

//Connectors
inline const unsigned int QUEUE_LENGTH_CONNECTIONS = 5;

//Message
inline const unsigned int CHUNK_SIZE = 8192;

//Header
inline const unsigned int HEADER_LENGTH = 9;

#endif //IMAGIC_BACKEND_CONSTANTS_H
