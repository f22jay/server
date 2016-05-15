/***************************************************************************
 *
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file channel.cpp
 * @author zhangfangjie(zhangfangjie@baidu.com)
 * @date 2016/01/07 19:44:06
 * @version $Revision$
 * @brief
 *
 **/

/* vim: set ts=4 sw=4 sts=4 tw=100 */
#include "net/channel.h"
const int Channel::NoneEvent = 0;
const int Channel::ReadEvent = POLLIN | POLLPRI;
const int Channel::WriteEvent = POLLOUT;

namespace net {
bool Channel::enableRead() {
  _events |= ReadEvent;
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

void Channel::remove() {
  _closeCallBack();
  _loop->removeChannel(this);
}

bool Channel::update() {
  return 0 == _loop->updateChannel(this);
}
int Channel::handleEvent() {
  if (_firedEvents & ReadEvent) {
    _readCallBack();
  }

  if (_firedEvents & WriteEvent) {
    _writeCallBack();
  }

  _firedEvents = 0;

}

}
