/* -*- C++ -*- */
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : moon (moon@baidu.com)
//
// Breif :listen port, accept connect request

#ifndef NET_ACCEPTOR_H
#define NET_ACCEPTOR_H
#include <memory>
#include "function.h"
namespace net {
class EventLoop;
class Channel;
class IpAddress;
class Socket;

class Acceptor {
 public:
  Acceptor(EventLoop* loop, NewConncetionCallack cb, IpAddress &address);
  virtual ~Acceptor();

 private:
  void handleRead();
  void setConnectionCallBack(NewConncetionCallack cb) {_connect_cb = cb;}

 private:
  Socket* _sock;
  EventLoop* _loop;
  std::unique_ptr<Channel> _channel;
  NewConncetionCallack _connect_cb;
};

} //namespace net
#endif
