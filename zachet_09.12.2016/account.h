//
// Created by ulyanin on 09.12.16.
//

#ifndef ZACHET_09_12_2016_ACCOUNT_H
#define ZACHET_09_12_2016_ACCOUNT_H

#include <atomic>
#include <mutex>
#include <condition_variable>

class account {
public:
    account()
    {
        sum_ = 0;
    }
    void deposit( unsigned int x) {
        std::unique_lock<std::mutex> lock_enter(mut_);
        sum_ += x;
        cv_.notify_all();
    }
    void withdraw(unsigned int y) {
        std::unique_lock<std::mutex> lock_enter(mut_);
        cv_.wait(lock_enter, [&] { return sum_ >= y; } );
        sum_ -= y;
    }

private:
    long long sum_;
    std::condition_variable cv_;
    std::mutex mut_;
};

#endif //ZACHET_09_12_2016_ACCOUNT_H
