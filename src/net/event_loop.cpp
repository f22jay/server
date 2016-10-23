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
    : _state(true), _poller(new EpollPoller()) {
    common::LOG_DEBUG("eventloop constructor");
}

EventLoop::~EventLoop() {
  common::LOG_DEBUG("eventloop distructor");
}

int EventLoop::updateChannel(Channel* channel) {
  return _poller->updateChannel(channel);
}

int EventLoop::removeChannel(Channel* channel) {
  return _poller->removeChannel(channel);
}

int EventLoop::poll(int timeout) {
  while (_state) {
    _activeList.clear();
    // assert(-1 != _poller->poll(timeout, &_activeList));
    _poller->poll(timeout, &_activeList);
    for (const auto& channel: _activeList) {
      channel->handleEvent();
    }
  }
}
}//namespace net
