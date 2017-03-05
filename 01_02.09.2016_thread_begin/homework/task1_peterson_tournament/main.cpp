#include "mutex_peterson.h"
#include "tree_mutex.h"
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

const int N_STEPS = 10;
int cnt = 0;
std::mutex test_mutex;

void function_to_test(tree_mutex *mut, int thread_index)
{
    for (int i = 0; i < N_STEPS; ++i) {
        mut->lock(thread_index);
        if (!test_mutex.try_lock()) {
            std::cerr << "error: lock failed" << std::endl;
        }
        // std::lock_guard<std::mutex> locker(mut);
        cnt++;
        test_mutex.unlock();
        mut->unlock(thread_index);
    }
}

void test(int n)
{
    tree_mutex mut(n);
    cnt = 0;
    std::vector<std::thread> threads;
    for (int i = 0; i < n; ++i) {
        threads.emplace_back(function_to_test, &mut, i);
    }
    for (int i = 0; i < n; ++i) {
        threads[i].join();
    }
    std::cout << "Testing ... OK" << std::endl;
}

int main()
{
    test(3);
    return 0;
}
