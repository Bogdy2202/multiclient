cmake_minimum_required(VERSION 3.28)
project(Client)

set(CMAKE_CXX_STANDARD 17)

add_executable(Client main.cpp
        Client.cpp
        Client.h)
