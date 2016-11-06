#include <iostream>
#include "barrier.h"
#include <vector>
#include <unistd.h>

std::mutex print_m;

void f(int thread_num, CyclicBarrier &b)
{
    for (int i = 0; i < 1000; ++i) {
        b.enter();
        print_m.lock();
        std::cout << i << " " << thread_num << std::endl;
        print_m.unlock();
    }
}

int main()
{

    int n = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    CyclicBarrier b(n);
    for (int i = 0; i < n; ++i) {
        threads.emplace_back(f, i, std::ref(b));
    }
    for (int i = 0; i < n; ++i) {
        threads[i].join();
    }
    return 0;
}