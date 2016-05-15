n/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

 /**
 * @file channel.h
 * @author moon(moon@baidu.com)
 * @date 2015/12/05 21:12:36
 * @version $Revision$
 * @brief
 *
 **/
#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>
#include <boost/function.hpp>
#include "event_loop.h"
namespace net {
class Channel{
public:
    Channel(int fd, Eventloop *eventloop, int events=0):_fd(fd),
                                                      _loop(eventloop),
                                                      _events(events)
                                                      _firedEvents(0){}

    bool enableRead();
    bool disableRead();
    bool enableWrite();
    bool disableWrite();
    void setReadCallBack(EventCallBack rCallBack);
    void setWriteCallBack(EventCallBack wCallBack);
    void setCloseCallBack(EventCallBack cCallBack);
    void remove();
    int getFd() { return _fd;}
    int getEvent() { return _events;}
    int handleEvent();
    void setFiredEvents(int event) { this->_firedEvents = event;}

private:
    bool update();
    typedef boost::function<void ()>  EventCallBack;
    static const int NoneEvent;
    static const int ReadEvent;
    static const int WriteEvent;

    int _fd;
    int _events;
    int _firedEvents;
    Eventloop *_loop;
    EventCallBack _readCallBack;
    EventCallBack _writeCallBack;
    EventCallBack _closeCallBack;
};
} //namesapce net
#endif  // CHANNEL_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
