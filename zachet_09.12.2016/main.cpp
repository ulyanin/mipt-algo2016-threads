#include <iostream>
#include "atomic_max.h"
#include "thread_safe_queue.h"
#include <thread>
#include <vector>
#include "account.h"


using namespace std;

const int N = 1000;

void f(int thread_num, thread_safe_queue<int> &q)
{
    for (int i = 0; i < N; ++i) {
        q.enqueue(i);
        cout << "push " << i << endl;
    }
}
void f2(int thread_num, thread_safe_queue<int> &q)
{
    for (int i = 0; i < N; ++i) {
        int val;
        q.pop(val);
        cout << "pop " << val << endl;
    }
}

int main()
{

//    int n = std::thread::hardware_concurrency();
    int n = 10000;
    thread_safe_queue<int> queue(N);
    std::vector<std::thread> threads;
    for (int i = 0; i < 1; ++i) {
//        f2(i, queue);
//        f(i, queue);
        threads.emplace_back(f, i, std::ref(queue));
        threads.emplace_back(f2, i, std::ref(queue));
    }
    cout << "s" << endl;
    sleep(2);
    cout << "shut" << endl;
    queue.shutdown();
    for (int i = 0; i < 2; ++i) {
        threads[i].join();
    }
    return 0;
}