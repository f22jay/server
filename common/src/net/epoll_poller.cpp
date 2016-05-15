/***************************************************************************
 *
 * Copyright (c) 2015 Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file epoll_poller.cpp
 * @author moon(f22jay@gmail.com)
 * @date 2015/12/26 17:08:53
 * @version $Revision$
 * @brief
 *
 **/
#include <sys/epoll.h>
#include "net/epoll_poller.h"

namespace net {

EpollPoller::EpollPoller() {
  _epfd = epoll_create1(0);
}

EpollPoller::~EpollPoller() {
  close(_epfd);
}

int EpollPoller::updateEvent(int op, Channel *channel) {
  struct epoll_event ee;
  ee.events = channel->getEvent();
  ee.data.u64 = 0;
  ee.data.fd = fd;
  return epoll_ctl(this->_epfd, op, fd, &ee);
}

int EpollPoller::updateChannel(Channel *channel) {
  int fd = channel->getFd();
  int op;
  if (_channelMap.find(fd) != _channelMap.end()) {
    op =EPOLL_CTL_MOD;
  }
  else {
    op = EPOLL_CTL_ADD;
    _channelMap.insert(std::make_pair(fd, channel));
  }
  return updateEvent(op, channel);
}

int EpollPoller::removeChannel(Channel *channel) {
  int fd = channel->getFd();
  if (_channelMap.find(fd) == _channelMap.end()) {
    return 0;
  }
  if ( -1 != updateEvent(EPOLL_CTL_DEL, channel)) {
    _channelMap.erase(_channelMap.find(fd));
    return 0;
  }
  else {
    return -1;
  }
}

int EpollPoller::poll(int timeout, ChannelList *active_channels) {
  int num_events = ::epoll_wait(_epfd,
                                &*(this->_eventList.begin()),
                                _eventList.size(), timeout);
  if (num_events < 0) {
    return num_events;
  }
  for (int i=0; i < num_events; i++) {
    int fired_fd = _eventList[i].data.fd;
    Channel *channel = _channelMap.find(fired_fd)->second;
    _channelMap.f
        channel->setFiredEvents(_eventList[i].events);
    active_channels->push_back(channel);
  }

  return num_events;
}
} //namespace net
