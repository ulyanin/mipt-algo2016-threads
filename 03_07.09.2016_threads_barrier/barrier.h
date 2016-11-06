#ifndef TASK_C_BARRIER_H
#define TASK_C_BARRIER_H

#include <thread>
#include <mutex>
#include <string>
#include <condition_variable>


#ifdef DEBUG
#include <iostream>

std::mutex m_print;

void print(const std::string &str)
{
    std::lock_guard<std::mutex> lock(m_print);
    std::cout << str << std::endl;
}
#endif


class IBarrier
{

public:
    virtual void enter() = 0;
    virtual ~IBarrier() {}
};

class CyclicBarrier : public IBarrier
{
public:
    CyclicBarrier(std::size_t num_threads);
    virtual void enter();
    ~CyclicBarrier(){};

protected:
    std::mutex mutex_enter_;
    std::condition_variable cv_enter_, cv_exit_;
    std::size_t entered_processes_, num_threads_;
    bool can_enter_, can_exit_;
};

CyclicBarrier::CyclicBarrier(std::size_t num_threads)
    : entered_processes_(0),
      num_threads_(num_threads),
      can_enter_(false),
      can_exit_(false)
{}


void CyclicBarrier::enter()
{
    // enter
    std::unique_lock<std::mutex> lock_enter(mutex_enter_);
    ++entered_processes_;
    if (entered_processes_ == num_threads_) {
        can_enter_ = true;
        can_exit_ = false;
        cv_enter_.notify_all();
    } else {
        cv_enter_.wait(lock_enter, [&] { return can_enter_; });
    }
    // exit
    --entered_processes_;
    if (entered_processes_ == 0) {
        can_enter_ = false;
        can_exit_ = true;
        cv_exit_.notify_all();
    } else {
        cv_exit_.wait(lock_enter, [&] { return can_exit_; });
    }
}

typedef CyclicBarrier barrier;

#endif //TASK_C_BARRIER_H
