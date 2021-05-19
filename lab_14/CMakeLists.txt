cmake_minimum_required(VERSION 3.19)
project(lab_14)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(lab_14 src/main.cpp)
add_executable(test test/smoke_test.cpp)