cmake_minimum_required(VERSION 3.20)
project(esecitazione1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE CXX FLAGS} -Wall -Wshadow -Werror=vla")


add_executable(esercizio_1 es1.cpp)
add_executable(esercizio_2 es2.cpp)