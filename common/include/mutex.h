/* -*- C++ -*- */
/*==================================
 *   Copyright (C) 2015 All rights reserved.
 *
 *   file  ： Mutex.h
 *   author： Zhang Fangjie (f22jay@163.com)
 *   date  ： 2015年11月04日
 *   desc  ：
 *
 ==================================*/
#pragma once

#include <pthread.h>

namespace common {
class Mutex {
 private:
  friend class CondVar;

  pthread_mutex_t _mutex;

 public:
  Mutex() {
    pthread_mutex_init(&_mutex, NULL);
  }

  ~Mutex() {
    pthread_mutex_destroy(&_mutex);
  }

  void Lock() {
    pthread_mutex_lock(&_mutex);
  }

  void UnLock() {
    pthread_mutex_unlock(&_mutex);
  }

  bool TryLock() {
    int ret = pthread_mutex_trylock(&_mutex);
    return ret == 0;
  }
};

class MutexGuard
{
 public:
  MutexGuard(Mutex* mtx): _mtx(mtx) {
    _mtx->Lock();
  }
  virtual ~MutexGuard() {
    _mtx->UnLock();
  }

 private:
  Mutex* _mtx;
};

}
