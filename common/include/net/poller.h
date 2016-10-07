/* -*- C++ -*-* /
/***************************************************************************
*
*  Copyright (c) 2015 Inc. All Rights Reserved
*  $Id$
*
* *************************************************************************/

/**
*  @file poller.h
*  @author moon(f22jay@gmail.com)
*  @date 2015/12/26 14:42:48
*  @version $Revision$
*  @brief
*
* */
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
  virtual int addChannel(Channel* channel) = 0;
  virtual int removeChannel(Channel* channel) = 0;
  virtual int poll(int timeout, ChannelList* active_channels) = 0;
  bool hasChannel(Channel* channel);

 private:
  ChannelMap _channelMap;
};
} //namespace net

#endif  // POLLER_H
