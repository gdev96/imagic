cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.)

add_executable(server
        constants.h
        image.cpp
        image.h
        load_balancer_connector.cpp
        load_balancer_connector.h
        main.cpp
        message.cpp
        message.h
        server.cpp
        server.h)
