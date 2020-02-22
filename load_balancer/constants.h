#ifndef LOAD_BALANCER_CONSTANTS_H
#define LOAD_BALANCER_CONSTANTS_H

#include <string>

// Output string
inline const std::string *OUTPUT_IDENTIFIER;

// Connectors
inline const unsigned int CONNECTIONS_QUEUE_SIZE = 10;

// Message
inline const unsigned int CHUNK_SIZE = 8192;

// Header
inline const unsigned int HEADER_LENGTH = 9;

#endif  // LOAD_BALANCER_CONSTANTS_H
