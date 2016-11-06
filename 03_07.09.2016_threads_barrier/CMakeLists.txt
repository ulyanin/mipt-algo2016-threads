cmake_minimum_required(VERSION 3.6)
project(task_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(SOURCE_FILES main.cpp)

find_package (Threads)
add_executable(task_c ${SOURCE_FILES})
target_link_libraries(task_c ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})