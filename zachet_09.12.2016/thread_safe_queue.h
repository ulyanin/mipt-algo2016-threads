//
// Created by ulyanin on 09.12.16.
//

#ifndef ZACHET_09_12_2016_THREAD_SAFE_QUEUE_H
#define ZACHET_09_12_2016_THREAD_SAFE_QUEUE_H

#include "semaphore.h"
#include <algorithm>
#include <deque>
#include <condition_variable>
#include <iostream>

class illegal_operation_exception: public std::exception
{
    virtual const char* what() const throw()
    {
        return "action with stopped thread_safe_queue";
    }
};

class MyMutex
{
public:
    MyMutex()
        : sem_mut_(1)
    { }
    void lock()
    {
        sem_mut_.aquire();
    }
    void unlock()
    {
        sem_mut_.release();
    }

private:
    semaphore sem_mut_;
};

template <class Value, class Container = std::deque<Value>>
class thread_safe_queue {
public:
    explicit thread_safe_queue(size_t capacity);
    ~thread_safe_queue()
    {}
    void enqueue(Value v);
    void pop(Value& v);
    void shutdown();
    bool empty() const;

protected:
    enum STATE { WORKING, STOPPED };
    MyMutex mutex_, mut_state_;
    STATE state_;
    size_t capacity_;
    Container queue_;
    semaphore sem_num_elements;
    semaphore sem_to_empty;
    bool queue_is_full_() const;
};


template <class Value, class Container>
thread_safe_queue<Value, Container>::thread_safe_queue(size_t capacity)
        : state_(WORKING)
        , capacity_(capacity)
        , sem_num_elements(capacity)
        , sem_to_empty(0)
{
    if (capacity == 0)
        throw std::exception();
}


template <class Value, class Container>
void thread_safe_queue<Value, Container>::enqueue(Value v)
{
    { /* catch mutex to read state_ */
        std::unique_lock<MyMutex> lock_enter(mutex_);
        if (state_ == STOPPED) {  /* must terminate */
            throw illegal_operation_exception();
        }
    }
    sem_num_elements.aquire();
    std::unique_lock<MyMutex> lock_enter(mutex_);
    if (state_ == STOPPED) {
        throw illegal_operation_exception();
    }
    queue_.push_back(std::move(v));
    sem_to_empty.release();
}

template <class Value, class Container>
void thread_safe_queue<Value, Container>::pop(Value &v)
{
    { /* catch mutex to read state_ */
        std::unique_lock<MyMutex> lock_enter(mutex_);
        if (state_ == STOPPED) {  /* must terminate */
            throw illegal_operation_exception();
        }
    }
    sem_to_empty.aquire();
    std::unique_lock<MyMutex> lock_enter(mutex_);
    if (state_ == STOPPED && queue_.empty()) {
        throw illegal_operation_exception();
    }
    v = std::move(queue_.front());
    queue_.pop_front();
    sem_num_elements.release();
}


template <class Value, class Container>
void thread_safe_queue<Value, Container>::shutdown()
{
    std::unique_lock<MyMutex> lock_enter(mutex_);
    state_ = STOPPED;
    for (size_t i = 0; i < capacity_; ++i) {
        /**
         * like notify
         */
        sem_to_empty.release();
        sem_num_elements.release();
    }
}

template <class Value, class Container>
bool thread_safe_queue<Value, Container>::empty() const
{
    return queue_.empty();
}

#endif //ZACHET_09_12_2016_THREAD_SAFE_QUEUE_H
