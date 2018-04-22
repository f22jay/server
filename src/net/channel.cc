// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/06 16:21:35
// Breif :

#include "channel.h"
#include <poll.h>
#include <sys/epoll.h>
#include "event_loop.h"
#include "log.h"

namespace net {
const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::WriteEvent = EPOLLOUT;

bool Channel::enableRead() {
  _events |= (ReadEvent | EPOLLET);
  return update();
}

bool Channel::disableRead() {
  _events &= ~ReadEvent;
  return update();
}

bool Channel::enableWrite() {
  _events &= ~WriteEvent;
  return update();
}
bool Channel::disableWrite() {
  _events &= ~WriteEvent;
  return update();
}

void Channel::setReadCallBack(EventCallBack rCallBack) {
  _readCallBack = rCallBack;
}

void Channel::setWriteCallBack(EventCallBack wCallBack) {
  _writeCallBack = wCallBack;
}

void Channel::setCloseCallBack(EventCallBack cCallBack) {
  _closeCallBack = cCallBack;
}

void Channel::remove() {
  _loop->removeChannel(this);
}

bool Channel::update() {
  return 0 == _loop->updateChannel(this);
}
int Channel::handleEvent() {
  if (_firedEvents & EPOLLHUP) {
    _closeCallBack();
    return 0;
  }

  if (_firedEvents & (EPOLLERR)) {
    return 0;
  }

  if (_firedEvents & ReadEvent) {
    _readCallBack();
  }

  if (_firedEvents & WriteEvent) {
    _writeCallBack();
  }

  _firedEvents = 0;
  return 0;
}

bool Channel::isWriting() {
  return _events & WriteEvent;
}

}//namespace net
