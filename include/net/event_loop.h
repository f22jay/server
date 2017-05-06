/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/01/07 18:51:52
// Breif :

#ifndef EPOLL_EVENT_H
#define EPOLL_EVENT_H
#include <memory>
#include <vector>
#include <functional>
#include "mutex.h"

namespace net{
class Channel;
class Poller;
class EventLoop{
 public:
  typedef std::vector<Channel* > ChannelList;
  typedef std::function<void ()> Func;

  EventLoop();
  ~EventLoop();

  int updateChannel(Channel* channel);
  int removeChannel(Channel* channel);

  // events loop
  int poll(int timeout = 1);
  void stop() {_state = false;}

  //run in loop thread, some ops in here, such as insert or delete channel map
  void runInLoop(const Func& func);

  void runPending();

 private:
  std::unique_ptr<Poller> _poller;
  bool _state;
  ChannelList _activeList;
  std::vector<Func> _pending_funcs;
  common::Mutex _mutex;
};
} //namespace net
#endif  // EPOLL_EVENT_H
