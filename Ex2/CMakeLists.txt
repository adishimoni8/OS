cmake_minimum_required(VERSION 3.21)
project(Ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ex2 Tests/test8new.cpp Exception.cpp QuantumTimer.cpp UThread.cpp uthreads.cpp)
