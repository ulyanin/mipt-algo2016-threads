#include "tree_mutex.h"
#include <iostream>

tree_mutex::tree_mutex(std::size_t num_threads)
{
    extended_num_threads_ = 1;
    while (extended_num_threads_ < num_threads) {
        extended_num_threads_ *= 2;
    }
    thread_offset_ = extended_num_threads_;
    mutexes_ = new MutexPeterson[extended_num_threads_];
}

tree_mutex::~tree_mutex()
{
    delete [] mutexes_;
}

void tree_mutex::lock(std::size_t thread_index)
{
    std::size_t vertex_index = thread_index + thread_offset_;
//    std::cout << "locking " << thread_index << std::endl;
    while (vertex_index != 1) {
        std::size_t mutex_index = vertex_index / 2;
//        std::cout << "\t" << mutex_index << " " << thread_index << " " << (vertex_index & 1) << std::endl;
        mutexes_[mutex_index].lock(vertex_index & 1);
        vertex_index /= 2;
    }
//    std::cout << "end locking " << thread_index << std::endl;
}

void tree_mutex::unlock(std::size_t thread_index)
{
    recursive_unlock_(thread_index + thread_offset_);
}

void tree_mutex::recursive_unlock_(std::size_t vertex_index)
{
    if (vertex_index == 1)
        return;
    recursive_unlock_(vertex_index / 2);
    std::size_t mutex_index = vertex_index / 2;
    mutexes_[mutex_index].unlock(vertex_index & 1);
}
