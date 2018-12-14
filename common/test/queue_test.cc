// All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2018/12/13 17:23:22
// Breif :

#include "lock_free_queue.h"
#include <vector>
#include <thread>
#include <atomic>
#include <iostream>
#include <unordered_set>

std::atomic<uint64_t> g_count(0);
common::LFCircleQueue<uint64_t> g_queue(1000 * 1024);

void produce_func(std::vector<uint64_t>* vi) {
    while (true) {
        uint64_t val = g_count.fetch_add(1);
        if (val >= 1000100) {
            break;
        }
        while (!g_queue.Push(val)) {
            sched_yield();
        }
        vi->push_back(val);
    }
}

void consume_func(std::vector<uint64_t>* vi) {
    while (true) {
        uint64_t val;
        while (!g_queue.Pop(&val)) {
            sched_yield();
        }
        vi->push_back(val);
        if (val >= 1000000) {
            break;
        }
    }
}
void check(const std::vector<std::vector<uint64_t>>& vvec) {
    std::unordered_set<uint64_t> umap;
    uint32_t cnt = 0;
    for (auto&& vec: vvec) {
        cnt += vec.size();
        umap.insert(vec.begin(), vec.end());
    }
    std::cout << cnt << std::endl;
    std::cout << umap.size() << std::endl;
};

int main(int argc, char *argv[]) {
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    int num = 1;
    std::vector<std::vector<uint64_t>> vvec(num);
    std::vector<std::vector<uint64_t>> rvvec(num);
    for (int i = 0; i < num; ++i) {
        producers.emplace_back(std::thread(produce_func, &rvvec[i]));
    }

    for (int i = 0; i < num; ++i) {
        consumers.emplace_back(std::thread(consume_func, &vvec[i]));
    }
    for (auto&& t: producers) {
        t.join();
    }
    for (auto&& t: consumers) {
        t.join();
    }
    check(vvec);
    check(rvvec);
    return 0;
}

