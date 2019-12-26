cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.
        /usr/include/mysql-cppconn-8)

set(PROJECT_LINK_LIBS libmysqlcppconn8.so)

add_executable(imagic_server
        constants.h
        image.cpp
        image.h
        load_balancer_connector.cpp
        load_balancer_connector.h
        main.cpp
        message.cpp
        message.h
        server.cpp
        server.h
        storage_manager.cpp
        storage_manager.h)

target_link_libraries(imagic_server ${PROJECT_LINK_LIBS})
