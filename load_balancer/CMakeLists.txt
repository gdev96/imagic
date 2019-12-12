cmake_minimum_required(VERSION 3.15)
project(imagic_backend)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(imagic_backend Main.cpp Connectors.cpp Connectors.h LoadBalancer.cpp LoadBalancer.h Constants.h Message.cpp Message.h)