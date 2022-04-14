cmake_minimum_required(VERSION 3.21)
project(Ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ex3 SampleClient.cpp mapReduceFramework.cpp ThreadContext.cpp JobContext.cpp Barrier.cpp Exception.h)

find_package (Threads)
target_link_libraries (Ex3 ${CMAKE_THREAD_LIBS_INIT})