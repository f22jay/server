/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/01/07 18:51:52
// Breif :

#ifndef EPOLL_EVENT_H
#define EPOLL_EVENT_H
#include <memory>
#include <vector>

namespace net{
class Channel;
class Poller;
class EventLoop{
 public:
  typedef std::vector<Channel* > ChannelList;

  EventLoop();
  ~EventLoop();

  int updateChannel(Channel* channel);
  int removeChannel(Channel* channel);
  int poll(int timeout = 1000);
  void stop() {_state = false;}

 private:
  std::unique_ptr<Poller> _poller;
  // Poller* _poller;
  bool _state;
  ChannelList _activeList;
};
} //namespace net
#endif  // EPOLL_EVENT_H
