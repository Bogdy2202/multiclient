cmake_minimum_required(VERSION 3.28)
project(ServerMulticlient)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerMulticlient main.cpp
        Server.cpp
        Server.h
        ClientHandler.cpp
        ClientHandler.h)
