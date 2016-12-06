// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/06 16:21:10
// Breif :

#include "poller.h"
#include "channel.h"

namespace net {
bool Poller::hasChannel(Channel *channel) {
  ChannelMap::const_iterator it = _channelMap.find(channel->get_fd());
  return it != _channelMap.end() && it->second == channel;
}
}//namesapce net
