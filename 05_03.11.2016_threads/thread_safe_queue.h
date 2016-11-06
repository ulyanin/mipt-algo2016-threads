#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <iostream>
#include <deque>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <exception>

class illegal_operation_exception: public std::exception
{
    virtual const char* what() const throw()
    {
        return "action with stopped thread_safe_queue";
    }
};


template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    explicit thread_safe_queue(size_t capacity);
    void enqueue(Value v);
    void pop(Value& v);

    /**
     * pop element from non empty queue
     * trying to pop from empty queue throws illegal_operation_exception
     * @param v
     */
    void try_pop(Value &v);
    void shutdown();
    bool empty() const;

protected:
    enum STATE { WORKING, STOPPED };
    std::mutex mutex_;
    std::atomic<STATE> state_;
    std::condition_variable cv_queue_is_empty_;
    std::condition_variable cv_queue_is_full_;
    std::condition_variable cv_all_done;
    size_t capacity_;
    Container queue_;

    bool queue_is_full_() const;
};

template <class Value, class Container>
bool thread_safe_queue<Value, Container>::queue_is_full_() const
{
    return queue_.size() == capacity_;
}

template <class Value, class Container>
thread_safe_queue<Value, Container>::thread_safe_queue(size_t capacity)
        : state_(WORKING)
        , capacity_(capacity)
{
    if (capacity == 0)
        throw std::exception();
}


template <class Value, class Container>
void thread_safe_queue<Value, Container>::enqueue(Value v)
{
    std::unique_lock<std::mutex> lock_enter(mutex_);
    cv_queue_is_full_.wait(lock_enter, [&] { return !queue_is_full_() || state_ == STOPPED; } );
    if (state_ == STOPPED) {
        throw illegal_operation_exception();
    }
    queue_.push_back(std::move(v));
    cv_queue_is_empty_.notify_one();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::pop(Value &v)
{
    std::unique_lock<std::mutex> lock_enter(mutex_);
    cv_queue_is_empty_.wait(lock_enter, [&] { return !queue_.empty() || state_ == STOPPED; } );
    if (queue_.empty() && state_ == STOPPED) {
        throw illegal_operation_exception();
    }
    v = std::move(queue_.front());
    queue_.pop_front();
    cv_queue_is_full_.notify_one();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::try_pop(Value &v)
{
    std::unique_lock<std::mutex> lock_enter(mutex_);
    if (queue_.empty()) {
        throw illegal_operation_exception();
    }
    v = std::move(queue_.front());
    queue_.pop_front();
    cv_queue_is_full_.notify_one();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::shutdown()
{
    if (state_ == STOPPED)
        return;
    state_ = STOPPED;
    cv_queue_is_full_.notify_all();
    cv_queue_is_empty_.notify_all();

}

template <class Value, class Container>
bool thread_safe_queue<Value, Container>::empty() const
{
    return queue_.empty();
}

#endif  // THREAD_SAFE_QUEUE_H