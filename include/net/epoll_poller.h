/* -*- C++ -*-* */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2015/12/26 16:17:23
// Breif :

#ifndef EPOLLPOLLER_H
#define EPOLLPOLLER_H
#include <sys/epoll.h>
#include <vector>
#include "poller.h"
#include "channel.h"

namespace net{
class EpollPoller : public Poller {
 public:
  EpollPoller();
  ~EpollPoller();
  virtual int updateChannel(Channel* channel);
  // virtual int addChannel(Channel* channel);
  virtual int removeChannel(Channel* channel);
  virtual int poll(int timeout, ChannelList* active_channels);
  int updateEvent(int op, Channel* channel);

 private:
  typedef std::vector<epoll_event> EventList;
  EventList _eventList;
  int _epfd;
};
} //namespace net

#endif  // EPOLLPOLLER_H
