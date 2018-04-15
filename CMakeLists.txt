cmake_minimum_required(VERSION 3.8)
project(GCP)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Solution.h Solution.cpp)
add_executable(GCP ${SOURCE_FILES})