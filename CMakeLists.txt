cmake_minimum_required(VERSION 3.10)
project(Cheap8)

if(UNIX)
        set(CMAKE_CXX_FLAGS "-g3 -Wall -O3 -pthread -lncurses")
endif ()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${Cheap8_SOURCE_DIR}/src)

file(GLOB Cheap8_SOURCES src/*)

add_executable(Cheap8 ${Cheap8_SOURCES})
