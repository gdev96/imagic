cmake_minimum_required(VERSION 3.15)
project(imagic_backend)

set(CMAKE_CXX_STANDARD 17)

add_executable(imagic_backend main.cpp Connector.cpp Connector.h LoadBalancer.cpp LoadBalancer.h)