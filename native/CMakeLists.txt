cmake_minimum_required(VERSION 3.17)
project(native C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "/GL /Os /GF /Gy /GA")
set(CMAKE_C_FLAGS_RELEASE "/GL /Os /GF /Gy /GA")

add_executable(native WIN32 main.c)
