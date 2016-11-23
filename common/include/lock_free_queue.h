//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2016/11/18 14:03:59
// Breif : mutil producer, single consume, producer may spin to enqueue
#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H
#include <atomic>
#include <vector>

namespace common {
template<T>
class lock_free_queue {
 public:
  lock_free_queue<T>(int size): size_(size), pool_(size), head_(0), _tail(0){}
  void enqueue(const T& t) {
    uint64_t tail = tail_.fetch_add(1);
    // if pool is full, while check
    while (tail - head_.load() >= size_);
    pool_[tail % size_] = t;
  }

  bool dequeue(T& t) {
    uint64_t head = head_;
    // if pool is empty, while check
    if (head == tail_.load()) {
      return false;
    }
    head_.fetch_add(1);
    t = pool_[head % size_];
    return true;
  }

 private:
  static int size_ = 10000;
  std::atomic<uint64_t> head_;
  std::atomic<uint64_t> tail_;
  std::vector<T> pool_;
}

}  // common

#endif
