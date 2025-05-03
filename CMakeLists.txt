cmake_minimum_required(VERSION 3.30)
project(pathplanning)

find_package(raylib REQUIRED)

add_executable(pathplanning main.cpp)

target_link_libraries(pathplanning PRIVATE raylib)
