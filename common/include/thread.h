/* -*- C++ -*- */
/*==================================
 *   Copyright (C) 2015 All rights reserved.
 *
 *   file  ： Thread.h
 *   author： Zhang Fangjie (f22jay@163.com)
 *   date  ： 2015年11月04日
 *   desc  ：
 *
 ==================================*/
#pragma once

#include <pthread.h>
#include <functional>

namespace common {
class Thread {
 public:
  typedef std::function<void()> Proc;

  bool Start(Proc proc) {
    _thread_func = proc;
    int ret = pthread_create(&_tid, NULL, Wraper, this);
    return ret == 0;
  }

  bool Start(void *func(void *), void *args) {
    int ret = pthread_create(&_tid, NULL, func, args);
    return ret == 0;
  }

  bool Join(void **rval_ptr = NULL) {
    int ret = pthread_join(_tid, rval_ptr);
    return ret == 0;
  }

 private:
  static void *Wraper(void *args) {
    (reinterpret_cast<Thread *>(args))->_thread_func();
  }

  Proc _thread_func;
  pthread_t _tid;
};
}
