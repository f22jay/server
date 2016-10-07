/* -*- C++ -*- */
/***************************************************************************
 *
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file epoll_event.h
 * @author zhangfangjie(zhangfangjie@baidu.com)
 * @date 2016/01/07 18:51:52
 * @version $Revision$
 * @brief
 *
 **/
#ifndef EPOLL_EVENT_H
#define EPOLL_EVENT_H
#include <memory>
#include <vector>
#include "epoll_poller.h"

namespace net{
class Channel;
class Poller;
class EventLoop{
 public:
  typedef std::vector<Channel* > ChannelList;
  EventLoop(bool run):_state(run),
                      _poller(new EpollPoller()) {}

  ~EventLoop() {}

  int updateChannel(Channel*  channel);
  int removeChannel(Channel* channel);
  int poll(int timeout, ChannelList* active_channels);

 private:
  std::unique_ptr<Poller> _poller;
  bool _state;
  ChannelList _activeList;
};
} //namespace net
#endif  // EPOLL_EVENT_H
