cmake_minimum_required(VERSION 3.15)
project(VectorSearchD03 CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(wyszukiwanie src/main.cpp src/loader.cpp src/wzorzec.cpp)

if(OpenMP_CXX_FOUND)
    target_link_libraries(wyszukiwanie PUBLIC OpenMP::OpenMP_CXX)
endif()

target_compile_options(wyszukiwanie PRIVATE -O3)