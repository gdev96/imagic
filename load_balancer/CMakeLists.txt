cmake_minimum_required(VERSION 3.15)
project(load_balancer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.)

add_executable(imagic_lb
        connectors.cpp
        connectors.h
        constants.h
        load_balancer.cpp
        load_balancer.h
        main.cpp
        message.cpp
        message.h)
