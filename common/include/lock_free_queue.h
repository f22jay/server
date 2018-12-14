//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2016/11/18 14:03:59
// Breif : mutil producer, single consume, producer may spin to enqueue
#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H
#include <stdint.h>
namespace common {
template<typename T>
class LFCircleQueue
{
 public:
  LFCircleQueue (uint64_t bits) {
    size_ = 1 << bits;
    mask_ = size_ - 1;
    buffers_ = new T[size_];
  }
  virtual ~LFCircleQueue() {
    if (buffers_ != nullptr) {
      delete[] buffers_;
    }
  }

  bool Push(const T& element) {
    uint64_t write_pos = 0, next_pos = 0;
    do {
      write_pos = write_;
      if (write_pos - read_ >= size_) {
        // full
        return false;
      }
      next_pos = write_pos + 1;
    } while (!__sync_bool_compare_and_swap(&write_, write_pos, next_pos));
    buffers_[write_pos & mask_] = element;
    return true;
  }

  bool Pop(T* element) {
    uint64_t read_pos = 0 , next_pos = 0;
    do {
      read_pos = read_;
      if (read_pos >= write_) {
        // empty
        return false;
      }
      next_pos = read_pos + 1;
    } while (!__sync_bool_compare_and_swap(&read_, read_pos, next_pos));
    *element = buffers_[read_pos & mask_];

    return true;
  }

 private:
  volatile uint64_t read_{0};
  volatile uint64_t write_{0};

  uint64_t size_{0};
  uint64_t mask_{0};
  T* buffers_{nullptr};
};

}  // common

#endif
