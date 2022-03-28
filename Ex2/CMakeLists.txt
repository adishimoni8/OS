cmake_minimum_required(VERSION 3.15)
project(Ex2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Ex2 main.cc uthreads.cc UThread.cc UThread.h Exception.h QuantumTimer.cc QuantumTimer.h Exception.cc EnvHelpers.cc)