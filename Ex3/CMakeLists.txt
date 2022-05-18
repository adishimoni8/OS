cmake_minimum_required(VERSION 3.1)

############################################################
#project(Ex3)
#
#set(CMAKE_CXX_STANDARD 11)
#
#add_executable(Ex3 SampleClient.cpp MapReduceFramework.cpp ThreadContext.cpp JobContext.cpp Exception.h Barrier.cpp)
#
#find_package (Threads)
#target_link_libraries (Ex3 ${CMAKE_THREAD_LIBS_INIT})


#############################################################

# NOTE: You can't have both ThreadSanitizer and AddressSanitizer enabled at the same time.

# Uncomment the following to enable ThreadSanitizer.
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=thread")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanittize=thread")

# Uncomment the following to enable AddressSanitizer.
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

# Project configuration
project(ex3 VERSION 1.0 LANGUAGES C CXX)

# Ensure system has pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(MapReduceFramework MapReduceClient.h MapReduceFramework.cpp MapReduceFramework.h Barrier.cpp)


set_property(TARGET MapReduceFramework PROPERTY CXX_STANDARD 11)
target_compile_options(MapReduceFramework PUBLIC -Wall -Wextra -O2 -fsanitize=address)
target_include_directories(MapReduceFramework PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# link pthreads to your framework
target_link_libraries(MapReduceFramework PUBLIC Threads::Threads -fsanitize=address)

# Add tests
add_subdirectory(mattanTests)

