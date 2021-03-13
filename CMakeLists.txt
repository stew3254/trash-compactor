cmake_minimum_required(VERSION 3.17)
project(compiler C)

set(CMAKE_C_STANDARD 99)

file(GLOB SOURCES src/*.h src/*.c)

add_executable(compiler ${SOURCES})