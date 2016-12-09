//
// Created by ulyanin on 09.12.16.
//

#ifndef ZACHET_09_12_2016_ATOMIC_MAX_H
#define ZACHET_09_12_2016_ATOMIC_MAX_H

#include <atomic>
#include <algorithm>

int fetch_and_set_max(std::atomic<int>& x, int y)
{
    int curr_x = x.load();
    while (!x.compare_exchange_weak(curr_x, std::max(curr_x, y)))
    { }
    return x.load();
}

#endif //ZACHET_09_12_2016_ATOMIC_MAX_H
