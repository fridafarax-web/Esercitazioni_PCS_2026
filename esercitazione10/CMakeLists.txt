cmake_minimum_required(VERSION 3.20)
project(esercitazione10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

# Trova la libreria Eigen3 nel sistema
find_package(Eigen3 REQUIRED)

enable_testing()

# 3. Definisce l'eseguibile principale "gcd"
add_executable(gcd main.cpp)

# 4. Collega Eigen e imposta le ottimizzazioni su "gcd"
target_link_libraries(gcd PRIVATE Eigen3::Eigen)

# 5. Registra il test dicend di lanciare direttamente l'eseguibile "gcd"
add_test(NAME gctest COMMAND gcd)