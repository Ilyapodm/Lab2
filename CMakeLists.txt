cmake_minimum_required(VERSION 3.24)
project(lab2 CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab2 
                src/main.cpp
        src/dynamic_array.tpp
        src/dynamic_array.hpp
        src/linked_list.hpp
        src/linked_list.tpp
        src/option.hpp
        src/sequence.hpp
        src/sequence.tpp)

target_compile_options(lab2 PRIVATE -Wall -Wextra)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)