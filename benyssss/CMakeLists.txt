cmake_minimum_required(VERSION 3.21)
project(benyssss)

set(CMAKE_CXX_STANDARD 14)

add_executable(benyssss SampleClient.cpp MapReduceFramework.cpp ErrorHandler.cpp ContextClasses.cpp Barrier.cpp)

find_package (Threads)
target_link_libraries (benyssss ${CMAKE_THREAD_LIBS_INIT})