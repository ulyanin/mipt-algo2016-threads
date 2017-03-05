cmake_minimum_required(VERSION 3.6)
project(zachet_09_12_2016)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lboost_system -lpthread")

set(SOURCE_FILES main.cpp account.h thread_safe_queue.h)
add_executable(zachet_09_12_2016 ${SOURCE_FILES})