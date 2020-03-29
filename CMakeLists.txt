cmake_minimum_required(VERSION 3.13)
project(raytracer)

set(CMAKE_CXX_STANDARD 14)

#main src directory
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MAIN_SOURCE)

add_executable(raytracer ${MAIN_SOURCE})

