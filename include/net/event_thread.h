//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2017/05/05 17:44:11
// Breif :

#ifndef INCLUDE_EVENT_THREAD_H
#define INCLUDE_EVENT_THREAD_H
#include "event_loop.h"
#include <memory>
#include "thread.h"
namespace net {
class EventLoopThread
{
 public:
  EventLoopThread(): _loop(NULL) {};
  virtual ~EventLoopThread() {}
  void set_loop(EventLoop* loop) {
    _loop = loop;
  }
  void Start() {
    _thread.Start(std::bind(&EventLoopThread::thread_run, this));
  }
  void Wait() {_thread.Join();}
  void thread_run() {
    _loop->poll();
  }
 private:
  EventLoop* _loop;
  common::Thread _thread;
};

class EventLoopThreadPool
{
 public:
  EventLoopThreadPool(int nums): _nums(nums), _events(0), _loops(nums), _threads(nums) {
    for (int i = 0; i < nums; i++) {
      _threads[i].set_loop(&_loops[i]);
    }
  };
  virtual ~EventLoopThreadPool() {}
  EventLoop* get_loop() {return &_loops[_events++ % _nums];}
  void Start() {
    for (auto& thread : _threads) {
      thread.Start();
    }
  }

  void Stop() {
    for (auto& loop : _loops) {
      loop.stop();
    }
  }

  void Wait() {
    for (auto& thread : _threads) {
      thread.Wait();
    }
  }
// void runInLoop(const Func& func) {getLoop()->runInLoop(func);}
 private:
  // nums threads and eventloop
  int _nums;
  unsigned long long _events;
  std::vector<EventLoop> _loops;
  std::vector<EventLoopThread> _threads;
};




}  // net
#endif
