cmake_minimum_required(VERSION 3.17)
project(lab_13)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
add_executable(lab_13 src/main.cpp include/my_array.h)
add_executable(smoke_compile src/smoke_compile.cpp include/my_array.h)