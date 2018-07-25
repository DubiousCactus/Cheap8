cmake_minimum_required(VERSION 3.10)
project(Cheap8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)

add_executable(Cheap8
        Chip8.cpp
        Chip8.h
        CPU.cpp
        CPU.h
        main.cpp
        Memory.cpp
        Memory.h
        Stack.cpp
        Stack.h)
