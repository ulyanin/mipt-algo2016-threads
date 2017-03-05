cmake_minimum_required(VERSION 3.0)
project(task1_peterson_tournament)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O2")
set(SOURCE_FILES main.cpp mutex_peterson.cpp tree_mutex.cpp)

find_package (Threads)
add_executable(task1_peterson_tournament ${SOURCE_FILES})
target_link_libraries(task1_peterson_tournament ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})