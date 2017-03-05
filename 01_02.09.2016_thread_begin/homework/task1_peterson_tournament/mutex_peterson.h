#ifndef I_MUTEX_PETERSON_H
#define I_MUTEX_PETERSON_H

#include <array>
#include <atomic>

class MutexPeterson {
public:
    MutexPeterson();
    MutexPeterson(MutexPeterson&&);
    void lock(int t);
    void unlock(int t);

private:
    std::atomic<bool> want_[2];
    std::atomic<int> victim_;
};

#endif
