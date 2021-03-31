cmake_minimum_required(VERSION 3.17)
project(lab_11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -O0 -fno-omit-frame-pointer -fsanitize=address")

include_directories(include)

file(GLOB_RECURSE HEADERS
        "${PROJECT_SOURCE_DIR}/include/*.h")

file(GLOB_RECURSE SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(lab_11 ${HEADERS} ${SOURCES})