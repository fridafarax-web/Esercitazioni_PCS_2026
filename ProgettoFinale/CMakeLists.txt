cmake_minimum_required(VERSION 3.20)
project(correnti_di_maglia CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla -Wno-sign-compare")

# Eigen e' usato per l'algebra lineare (come nell'esercitazione 10).
find_package(Eigen3 REQUIRED)

# Tutto il codice e' header-only: ogni eseguibile compila un solo .cpp.

# ----- Eseguibile principale -----
add_executable(CorrentiDiMaglia main.cpp)
target_link_libraries(CorrentiDiMaglia Eigen3::Eigen)

# ----- Test -----
enable_testing()

add_executable(test_netlist test_netlist.cpp)
add_test(NAME netlist COMMAND test_netlist)

add_executable(test_cycles test_cycles.cpp)
target_link_libraries(test_cycles Eigen3::Eigen)
add_test(NAME cycles COMMAND test_cycles)

add_executable(test_solver test_solver.cpp)
target_link_libraries(test_solver Eigen3::Eigen)
add_test(NAME solver COMMAND test_solver)

add_executable(test_gradiente test_gradiente.cpp)
target_link_libraries(test_gradiente Eigen3::Eigen)
add_test(NAME gradiente COMMAND test_gradiente)

add_executable(test_lifofifo test_lifofifo.cpp)
add_test(NAME lifofifo COMMAND test_lifofifo)

add_executable(test_matrici test_matrici.cpp)
target_link_libraries(test_matrici Eigen3::Eigen)
add_test(NAME matrici COMMAND test_matrici)
