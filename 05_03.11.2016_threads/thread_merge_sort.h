/**
 * @author ulyanin
 */

#ifndef THREADS_THREAD_MERGE_SORT_H
#define THREADS_THREAD_MERGE_SORT_H

#include <algorithm>

#include "thread_pool.h"

class parallel_sort
{
public:
    parallel_sort(std::size_t num_workers);

    template <class RandomAccessIterator, class Compare>
    void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

protected:
    template <class RandomAccessIterator, class Compare>
    void merge_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

    thread_pool<void> thread_pool_;
    static const std::ptrdiff_t MIN_CHUNK_SIZE = 2;
};


parallel_sort::parallel_sort(std::size_t num_workers)
    : thread_pool_(num_workers)
{ }


template <class RandomAccessIterator, class Compare>
void parallel_sort::merge_sort(RandomAccessIterator first,
                                 RandomAccessIterator last,
                                 Compare comp)
{
    std::ptrdiff_t length = std::distance(first, last);
    if (length < MIN_CHUNK_SIZE) {
        std::sort(first, last, comp);
        return;
    }
    RandomAccessIterator middle = first;
    std::advance(middle, length / 2);  // middle += length / 2

    std::function<void()> left_part = [&] () -> void { merge_sort(first, middle, comp); };
    std::function<void()> right_part = [&] () -> void { merge_sort(middle, last, comp); };

    std::future<void> future_left = thread_pool_.submit(std::move(left_part));
    thread_pool_.wait(std::move(future_left));
    right_part();

    std::vector<typename RandomAccessIterator::value_type> buffer(length);
    std::merge(first, middle, middle, last, buffer.begin(), comp);
    std::copy(buffer.begin(), buffer.end(), first);
//    std::inplace_merge(first, middle, last, comp);
}


template <class RandomAccessIterator, class Compare>
void parallel_sort::sort(RandomAccessIterator first,
                           RandomAccessIterator last,
                           Compare comp)
{
    std::function<void()> sort_func = std::bind(&parallel_sort::merge_sort<RandomAccessIterator, Compare>,
                                                this, first, last, comp);
    std::future<void> future_sorter = thread_pool_.submit(std::move(sort_func));
    future_sorter.wait();
    future_sorter.get();
    thread_pool_.shutdown();
}


#endif //THREADS_THREAD_MERGE_SORT_H
