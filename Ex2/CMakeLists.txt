cmake_minimum_required(VERSION 3.15)
project(Ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ex2 main.cc uthreads.cpp UThread.cpp UThread.h Exception.h QuantumTimer.cpp QuantumTimer.h Exception.cpp EnvHelpers.h)