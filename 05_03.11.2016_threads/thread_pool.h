#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <future>
#include <condition_variable>
#include <vector>
#include <chrono>

#include "thread_safe_queue.h"

template <class Value>
class thread_pool {
public:
    explicit thread_pool(size_t num_workers);
    std::future<Value> submit(std::function<Value()> task);
    void shutdown();
    /**
     * this method run tasks from thread_pool while
     * waiting for future<Value> task_for_wait
     */
    void wait(std::future<Value> &&task_for_wait);

protected:
    void thread_worker_();
    void active_execute();

    thread_safe_queue<std::packaged_task<Value()>> queue_;
    std::vector<std::thread> workers_;
    std::condition_variable cv_task_;
};

template <class Value>
thread_pool<Value>::thread_pool(size_t num_workers)
    : queue_(-1)
{
    for (std::size_t i = 0; i < num_workers; ++i) {
        workers_.emplace_back([&] { return thread_worker_(); });
    }
}

template <class Value>
void thread_pool<Value>::thread_worker_()
{
    while (true) {
        try {
            std::packaged_task<Value()> current_task;
            queue_.pop(current_task);
            current_task();
        } catch (illegal_operation_exception &e) {
            break;
        }
    }
}

template <class Value>
void thread_pool<Value>::shutdown()
{
    queue_.shutdown();
    for (std::thread& t : workers_) {
        t.join();
    }
}

template <class Value>
std::future<Value> thread_pool<Value>::submit(std::function<Value()> task)
{
    std::packaged_task<Value()> pck_task(task);
    std::future<Value> future_result = pck_task.get_future();
    queue_.enqueue(std::move(pck_task));
    return future_result;
}

template <class Value>
void thread_pool<Value>::wait(std::future<Value> &&task_for_wait)
{
    while (task_for_wait.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
        active_execute();
    }
}

template <class Value>
void thread_pool<Value>::active_execute()
{
    try {
        std::packaged_task<Value()> current_task;
        queue_.try_pop(current_task);
        current_task();
    } catch (illegal_operation_exception &e) {
        return;
    }
}

#endif  // THREAD_POOL_H