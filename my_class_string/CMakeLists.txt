cmake_minimum_required(VERSION 3.15)
project(my_class_string)

set(CMAKE_CXX_STANDARD 14)

add_executable(my_class_string main.cpp my_string.h my_string.cpp)