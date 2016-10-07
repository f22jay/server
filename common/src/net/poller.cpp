/***************************************************************************
 *
 * Copyright (c) 2015 Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file poller.c
 * @author moon(f22jay@gmail.com)
 * @date 2015/12/26 15:36:57
 * @version $Revision$
 * @brief
 *
 **/

/* vim: set ts=4 sw=4 sts=4 tw=100 */
#include "poller.h"
#include "channel.h"

namespace net {
bool Poller::hasChannel(Channel *channel) {
  ChannelMap::const_iterator it = _channelMap.find(channel->getFd());
  return it != _channelMap.end() && it->second == channel;
}
}//namesapce net
