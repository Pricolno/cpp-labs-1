cmake_minimum_required(VERSION 3.19)
project(lab_14)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

include_directories(include)
add_executable(lab_14 src/main.cpp)
add_executable(smoke_test test/smoke_test.cpp)
add_executable(unit_test test/test.cpp)