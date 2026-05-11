cmake_minimum_required(VERSION 3.20)
project(esercitazione8)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")
# Creo il target per l'eseguibile principale del mio progetto software
add_executable(graphs main.cpp)
## Creo i target per gli eseguibili che effettueranno i test unitari
add_executable(graphtest graphtest.cpp)
add_test(NAME testgraphs COMMAND graphtest)
enable_testing()