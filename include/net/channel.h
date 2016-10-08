/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2015/12/05 21:12:36
// Breif :

#ifndef CHANNEL_H
#define CHANNEL_H
#include "function.h"
/* class Eventloop; if exisit here, is error, not in net */

namespace net {
class EventLoop;

class Channel {
 public:
 Channel(int fd, EventLoop* eventloop, int events = 0)
      : _fd(fd),
        _loop(eventloop),
        _events(events),
        _firedEvents(0) {}

  bool enableRead();
  bool disableRead();
  bool enableWrite();
  bool disableWrite();
  void setReadCallBack(EventCallBack rCallBack);
  void setWriteCallBack(EventCallBack wCallBack);
  void setCloseCallBack(EventCallBack cCallBack);
  void remove();
  void disableAll() {_events = NoneEvent; update();}
  bool isWriting();
  int get_fd() { return _fd;}
  int get_event() { return _events;}
  int handleEvent();
  void setFiredEvents(int event) { this->_firedEvents = event;}

 private:
  bool update();
 private:
  static const int NoneEvent;
  static const int ReadEvent;
  static const int WriteEvent;
  int _fd;
  int _events;
  int _firedEvents;
  EventLoop* _loop;
  EventCallBack _readCallBack;
  EventCallBack _writeCallBack;
  EventCallBack _closeCallBack;
};
} //namesapce net
#endif  // CHANNEL_H
