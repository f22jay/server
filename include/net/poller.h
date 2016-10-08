/* -*- C++ -*-*/
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2015/12/26 14:42:48
// Breif :

#ifndef POLLER_H
#define POLLER_H
#include <map>
#include <vector>
namespace net{
class Channel;
class Poller {
 public:
  typedef std::vector<Channel* > ChannelList;
  typedef std::map<int, Channel* > ChannelMap;

  Poller() {}
  virtual ~Poller() {}

  virtual int updateChannel(Channel* channel) = 0;
  // virtual int addChannel(Channel* channel) = 0;
  virtual int removeChannel(Channel* channel) = 0;
  virtual int poll(int timeout, ChannelList* active_channels) = 0;
  bool hasChannel(Channel* channel);

 protected:
  ChannelMap _channelMap;
};
} //namespace net

#endif  // POLLER_H
