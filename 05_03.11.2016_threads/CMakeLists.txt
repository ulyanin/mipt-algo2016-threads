cmake_minimum_required(VERSION 3.6)
project(05_03_11_2016_threads)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

add_executable(05_03_11_2016_threads ${SOURCE_FILES})
target_link_libraries(05_03_11_2016_threads Threads::Threads)
