//
// Created by ulyanin on 09.12.16.
//

#ifndef ZACHET_09_12_2016_SEMAPHORE_H
#define ZACHET_09_12_2016_SEMAPHORE_H

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

class semaphore {
public:
    boost::interprocess::interprocess_semaphore sem;
    explicit semaphore(int initial)
        : sem(initial)
    { }
    void aquire()
    {
        sem.wait();
    }
    void release()
    {
        sem.post();
    }
};

#endif //ZACHET_09_12_2016_SEMAPHORE_H
