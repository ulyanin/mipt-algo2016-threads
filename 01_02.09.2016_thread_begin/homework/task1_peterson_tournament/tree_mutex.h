#include "mutex_peterson.h"
#include <vector>
#include <atomic>

class tree_mutex
{
public:
    tree_mutex(std::size_t num_threads);
    ~tree_mutex();
    void lock(std::size_t thread_index);
    void unlock(std::size_t thread_index);
private:
    std::size_t thread_offset_, extended_num_threads_;
    MutexPeterson * mutexes_;

    void recursive_unlock_(std::size_t vertex_index);
};
