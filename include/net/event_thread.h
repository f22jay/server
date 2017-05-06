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
  EventLoopThread(EventLoop* loop): _loop(loop) {};
  virtual ~EventLoopThread() {}
  void Start() {_thread.Start(std::bind(&EventLoopThread::thread_run, this));}
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
  EventLoopThreadPool(int nums): _nums(nums), _events(0), _loops(nums), _threads(nums) {};
  virtual ~EventLoopThreadPool() {}
  EventLoop* getLoop() {return &_loops[_events++ % _nums];}
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
