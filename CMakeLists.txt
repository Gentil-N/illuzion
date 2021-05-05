cmake_minimum_required(VERSION 3.0)

project(illuzion)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/code/)

set(SRC_FILES ${SRC_DIR}/main.cpp
              ${SRC_DIR}/stb_image.c
              ${SRC_DIR}/stb_image_write.c
              ${SRC_DIR}/picture.cpp
              ${SRC_DIR}/effect.cpp)

add_executable(illuzion ${SRC_FILES})
file(COPY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})