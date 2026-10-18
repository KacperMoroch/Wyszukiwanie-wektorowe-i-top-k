cmake_minimum_required(VERSION 3.15)
project(VectorSearchD03 CXX)

set(CMAKE_CXX_STANDARD 17)

# Tu w przyszłości odkomentujemy OpenMP, MPI i CUDA

add_executable(wyszukiwanie src/main.cpp src/loader.cpp)
