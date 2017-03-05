#include <thread>
#include <iostream>
#include <vector>
#include <mutex>

int cnt = 0;
int N_STEPS = 10000000;
std::mutex mut;

template <class Mutex>
class MutexWrapperLock
{
public:
    MutexWrapperLock(Mutex &m)
        : mut_ref(m)
    {
        m.lock();
    }
    ~MutexWrapperLock()
    {
        mut_ref.unlock();
    }
    Mutex &mut_ref;
};

void thread_increment() {
    for (int i = 0; i < N_STEPS; ++i) {
        //MutexWrapperLock<std::mutex> locker(mut);
        std::lock_guard<std::mutex> locker(mut);
        cnt++;
    }
}


int main()
{
    std::vector<std::thread> threads;
    int n_threads = std::thread::hardware_concurrency();
    std::cout << n_threads << std::endl;
    for (int i = 0; i < n_threads; ++i) {
        threads.emplace_back(thread_increment);
    }
    for (int i = 0; i < n_threads; ++i) {
        threads[i].join();
    }
    std::cout << cnt << std::endl;
    return 0;
}
