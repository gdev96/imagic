cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(server
        Connectors.cpp
        Connectors.h
        Constants.h
        LoadBalancer.cpp
        LoadBalancer.h
        main.cpp
        Message.cpp
        Message.h Image.cpp Image.h)
