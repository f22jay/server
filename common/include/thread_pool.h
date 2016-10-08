/* -*- C++ -*- */
/*==================================
 *   Copyright (C) 2015 All rights reserved.
 *
 *   file  ： thread_pool.h
 *   author： Zhang Fangjie (f22jay@163.com)
 *   date  ： 2015年11月12日
 *   desc  ：
 *
 ==================================*/
#pragma once

#include <stdint.h>
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <string>
#include <functional>
#include <map>
#include "mutex.h"
#include "cond_var.h"
#include "thread.h"

namespace common {
struct Task{
  typedef std::function<void()> Func;
  uint64_t _priority;
  Func _func;
  uint64_t _task_id;
  bool _running;
  Task(Func func, uint64_t priority = 0): _func(func),
                                          _priority(priority),
                                          _running(false),
                                          _task_id(0){}
  friend bool operator< (const Task &lhs, const Task &rhs);
};
bool operator< (const Task &lhs, const Task &rhs) {
  return lhs._priority <rhs._priority;
}

class ThreadPool {
 public:
  ThreadPool(uint64_t thread_num):
      _thread_num(thread_num),
      _condVar(&_mutex),
      _task_ids(0),
      _run_flag(true),
      _task_pending(0) {}

  void Start() {
    uint64_t num = 0;
    pthread_t tid;
    while (num++ < _thread_num) {
      int ret = pthread_create(&tid, NULL, WrapperRun , this);
      if (ret != 0) {
        exit(-1);
      }
      _thread_pool.push_back(tid);
    }
  }

  void Stop(){
    _mutex.Lock();
    _run_flag = false;
    _mutex.UnLock();
    for(std::vector<pthread_t>::const_iterator
            iterator = _thread_pool.begin();
        iterator != _thread_pool.end();
        iterator++) {
      pthread_join(*iterator, NULL);
    }
    _thread_pool.clear();
  }

  uint64_t AddTask(void func(), uint64_t priority = 0) {
    uint64_t id;
    _mutex.Lock();
    Task task(func, priority);
    _task_queue.push(task);
    id = _task_ids++;
    std::pair<uint64_t, Task> id_task = std::make_pair(id, task);
    _task_map.insert(id_task);
    _mutex.UnLock();
    _condVar.Signal();
    return id;
  }

  static void * WrapperRun(void * p) {
    (reinterpret_cast<ThreadPool *>(p))->Run();
    return NULL;
  }

  void Run() {
    while (true) {
      _mutex.Lock();
      if (_run_flag == false) {
        _mutex.UnLock();
        break;
      }
      while (_task_queue.empty()) {
        _condVar.Wait();
      }
      Task task = _task_queue.top();
      std::cout<<_task_queue.size()<<std::endl;
      _task_queue.pop();
      _mutex.UnLock();
      task._func();
    }
  }

 private:
  Mutex _mutex;
  CondVar _condVar;
  uint64_t _thread_num;
  uint64_t _task_ids;
  uint64_t _task_pending;
  std::string _pool_name;
  std::vector<pthread_t> _thread_pool;
  std::priority_queue<Task> _task_queue;
  std::map<uint64_t, Task> _task_map;
  bool _run_flag;
};
}
