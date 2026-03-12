cmake_minimum_required(VERSION 3.20)
project(esecitazione1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(temp_media medie.cpp)
add_executable(prova printargs.cpp)