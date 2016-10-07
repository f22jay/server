/* -*- C++ -*- */
/*==================================
 *   Copyright (C) 2015 All rights reserved.
 *
 *   file  ： CondVar.h
 *   author： Zhang Fangjie (f22jay@163.com)
 *   date  ： 2015年11月07日
 *   desc  ：
 *
 ==================================*/
#pragma once

#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include "mutex.h"

namespace common {
class CondVar {
 private:
  Mutex *_mutex;
  pthread_cond_t _cond;

 public:
  CondVar(Mutex *mu) : _mutex(mu) {
    pthread_cond_init(&_cond, NULL);
  }

  ~CondVar() {
    pthread_cond_destroy(&_cond);
  }

  void Wait() {
    pthread_cond_wait(&_cond, &_mutex->_mutex);
  }

  bool TimeWait(uint64_t millseconds) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += millseconds / 1000;
    ts.tv_nsec += (millseconds % 1000) * 1000 * 1000;

    int ret = pthread_cond_timedwait(&_cond, &_mutex->_mutex, &ts);
    return ret == 0;
  }

  void Signal() {
    pthread_cond_signal(&_cond);
  }

  void BroadCast() {
    pthread_cond_broadcast(&_cond);
  }

};
}
