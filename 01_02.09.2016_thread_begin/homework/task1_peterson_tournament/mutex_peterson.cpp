#include "mutex_peterson.h"
#include <thread>

MutexPeterson::MutexPeterson() {
    want_[0].store(false);
    want_[1].store(false);
    victim_.store(0);
}

MutexPeterson::MutexPeterson(MutexPeterson &&other)
{
    for (int i = 0; i < 2; ++i)
        want_[0].store(std::move(other.want_[i]));
    victim_.store(std::move(other.victim_));
}

void MutexPeterson::lock(int t) {
    want_[t].store(true);
    victim_.store(t);
    while (want_[t ^ 1].load() && victim_.load() == t) {
        std::this_thread::yield();
    }
}

void MutexPeterson::unlock(int t) {
    want_[t].store(false);
}
