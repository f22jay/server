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
#include <boost/scoped_ptr.hpp>
#include "channel.h"
#include "poller.h"

namespace net{
class EventLoop{
public:
    Eventloop(bool run):_state(run),
                        _poller(new EpollPoller()) {}

    ~Eventloop() {}

    int updateChannel(Channel * channel);
    int removeChannel(Channel *channel);
    int poll(int timeout, ChannelList *active_channels);

private:
    typedef std::vector<Channel *> ChannelList;
    boost::scoped_ptr<Poller> _poller;
    bool _state;
    ChannelList _activeList;

};
} //namespace net
#endif  // EPOLL_EVENT_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
