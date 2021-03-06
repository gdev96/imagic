cmake_minimum_required(VERSION 3.15)
project(server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.
        /usr/include/mysql-cppconn-8
        /usr/local/include/ImageMagick-7)

set(PROJECT_LINK_LIBS
        /usr/lib/x86_64-linux-gnu/libmysqlcppconn8.so
        /usr/local/lib/libMagick++-7.Q16HDRI.so)

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

if(TESTING_MACRO)
    add_definitions(-DTESTING)
endif(TESTING_MACRO)

add_executable(imagic_srv
        constants.h
        image.cpp
        image.h
        load_balancer_connector.cpp
        load_balancer_connector.h
        main.cpp
        message.cpp
        message.h
        payload.cpp
        payload.h
        server.cpp
        server.h
        socket.cpp
        socket.h
        storage_manager.cpp
        storage_manager.h)

target_link_libraries(imagic_srv ${PROJECT_LINK_LIBS})
