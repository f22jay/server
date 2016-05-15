/***************************************************************************
 *
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file event_loop.cpp
 * @author zhangfangjie(zhangfangjie@baidu.com)
 * @date 2016/01/07 19:23:49
 * @version $Revision$
 * @brief
 *
 **/

/* vim: set ts=4 sw=4 sts=4 tw=100 */
#include "net/event_loop.h"

namespace net {

int EventLoop::updateChannel(Channel *channel) {
  return _poller->updateChannel(channel);
}

int EventLoop::removeChannel(Channel *channel) {
  return _poller->removeChannel(channel);
}

int EventLoop::poll(int timeout) {
  while (_state) {
    int ret = _poller->poll(timeout, &_activeList);
    if (ret != 0) {
      return -1;        /**<  need do something      */
    }

    for (ChannelList::iterator it = _activeList.begin();
         it != _activeList.end(); it++) {
      (*it)->handleEvent();
    }
  }
}
}//namespace net
