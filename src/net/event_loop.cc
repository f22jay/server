// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/01/07 19:23:49
// Breif :

#include "event_loop.h"
#include <assert.h>
#include "channel.h"
#include "epoll_poller.h"
#include "log.h"

namespace net {
EventLoop::EventLoop()
    : _state(true), _poller(new EpollPoller()) {}

EventLoop::~EventLoop() {}

int EventLoop::updateChannel(Channel* channel) {
  return _poller->updateChannel(channel);
}

int EventLoop::removeChannel(Channel* channel) {
  return _poller->removeChannel(channel);
}

void EventLoop::runInLoop(const Func& func) {
  common::MutexGuard guard(&_mutex);
  _pending_funcs.push_back(func);
}

void EventLoop::runPending() {
  std::vector<Func> pending_funcs;
  {
    common::MutexGuard guard(&_mutex);
    pending_funcs.swap(_pending_funcs);
  }

  for (auto func: pending_funcs) {
    func();
  }
}

int EventLoop::poll(int timeout) {
  while (_state) {
    _activeList.clear();
    _poller->poll(timeout, &_activeList);
    for (const auto& channel: _activeList) {
      channel->handleEvent();
    }

    runPending();
  }
}
}//namespace net
