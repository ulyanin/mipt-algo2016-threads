#include <iostream>
//#include "thread_safe_queue.h"
//#include "thread_pool.h"
#include "thread_merge_sort.h"
#include <cstdlib>
#include <algorithm>
#include <stdlib.h>
#include <functional>

int main(int argc, char ** argv)
{
    int n = 1 * 1000000;
    std::vector<int> v(n);
    std::generate(v.begin(), v.end(), [] { return rand() % 100; } );
//    for (int i : v) {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl;
    if (argc > 1 && argv[1][0] == '1') {
        std::cout << "std::sort" << std::endl;
        std::sort(v.begin(), v.end());
    } else {
        std::cout << "parallel_sort" << std::endl;
        parallel_sort sorter(4);
        sorter.sort(v.begin(), v.end(), std::less<int>());
        for (int i = 1; i < v.size(); ++i) {
            if (v[i] < v[i - 1]) {
                std::cerr << "ERR" << std::endl;
            }
        }
    }
//    for (int i : v) {
//        std::cout << i << " ";
//    }
//    std::cout << std::endl;

    return 0;
}