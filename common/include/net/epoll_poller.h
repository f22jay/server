/***************************************************************************
 *
 * Copyright (c) 2015 Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

 /**
 * @file EpollPoller.h
 * @author moon(f22jay@gmail.com)
 * @date 2015/12/26 16:17:23
 * @version $Revision$
 * @brief
 *
 **/
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
    virtual int updateChannel(Channel *channel);
    virtual int addChannel(Channel *channel);
    virtual int removeChannel(Channel *channel);
    virtual int poll(int timeout, ChannelList *active_channels);
    int updateEvent(int op, Channel *channel);

private:
    typedef std::vector<epoll_event> EventList;
    EventList _eventList;
    int _epfd;
};
} //namespace net

#endif  // EPOLLPOLLER_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */









