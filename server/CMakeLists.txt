cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(server
        Constants.h
        main.cpp
        Message.cpp
        Message.h Image.cpp Image.h Server.cpp Server.h ConnectorLoadBalancer.cpp ConnectorLoadBalancer.h)
