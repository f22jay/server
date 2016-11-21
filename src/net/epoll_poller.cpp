// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2015/12/26 17:08:53
// Breif :

#include "epoll_poller.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <atomic>
#include "log.h"

namespace net {
const int kEventInitialSize = 32;
std::atomic<unsigned long long>  g_event_num(0);
EpollPoller::EpollPoller(): _eventList(kEventInitialSize){
  // _epfd = epoll_create1(0);
  _epfd = epoll_create(1024);

}

EpollPoller::~EpollPoller() {
  close(_epfd);
}

int EpollPoller::updateEvent(int op, Channel* channel) {
  struct epoll_event ee;
  int fd = channel->get_fd();
  ee.events = channel->get_event();
  ee.data.u64 = 0;
  ee.data.fd = fd;
  // common::LOG_DEBUG("update _epfd[%d] op[%d] fd[%d] events[%d]", _epfd, op, fd, ee.events);
  return epoll_ctl(this->_epfd, op, fd, &ee);
}

int EpollPoller::updateChannel(Channel* channel) {
  int fd = channel->get_fd();
  int op;
  if (_channelMap.find(fd) != _channelMap.end()) {
    op =EPOLL_CTL_MOD;
  } else {
    op = EPOLL_CTL_ADD;
    _channelMap.insert(std::make_pair(fd, channel));
     // common::LOG_INFO("add channel fd[%d] suc", fd);
 }
  return updateEvent(op, channel);
}

int EpollPoller::removeChannel(Channel* channel) {
  int fd = channel->get_fd();
  ChannelMap::const_iterator it = _channelMap.find(fd);
  if (it == _channelMap.end()) {
    return 0;
  }
  if ( 0 == updateEvent(EPOLL_CTL_DEL, channel)) {
    _channelMap.erase(it);
    // common::LOG_INFO("remove channel fd[%d] suc", fd);
    return 0;
  } else {
    // common::LOG_INFO("remove channel fd[%d] fail", fd);
    return -1;
  }
}

int EpollPoller::poll(int timeout, ChannelList* active_channels) {
  assert(_eventList.size() > 0);
  assert(_epfd > 0);
  int num_events = ::epoll_wait(_epfd,
                                &*(this->_eventList.begin()),
                                _eventList.size(),
                                timeout);
  if(num_events < 0) {
    common::LOG_FATAL("%s", strerror(errno));
    return -1;
  }
  // common::LOG_INFO("epoll num_events[%d]", num_events);
  for (int i = 0; i < num_events; i++) {
    int fired_fd = _eventList[i].data.fd;
    auto iter = _channelMap.find(fired_fd);
    if (iter == _channelMap.end()) {
      continue;
    }
    Channel* channel = iter->second;
    channel->setFiredEvents(_eventList[i].events);
    active_channels->push_back(channel);
  }

  if (num_events == _eventList.size()) {
    _eventList.resize(num_events * 2);
  }
  g_event_num.fetch_add(num_events);
  return num_events;
}
} //namespace net
