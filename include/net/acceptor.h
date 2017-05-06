/* -*- C++ -*- */
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : moon (moon@baidu.com)
// Date 2016/03/08 17:39:32
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
  Acceptor(EventLoop* loop, NewConncetionCallack cb, const IpAddress &address);
  virtual ~Acceptor();
  EventLoop* get_loop() {return _loop;}
  void start();
 private:
  void handleRead();
  void handleClose();
  void setConnectionCallBack(NewConncetionCallack cb) {_connect_cb = cb;}

 private:
  EventLoop* _loop;
  std::unique_ptr<Socket> _sock;
  std::unique_ptr<Channel> _channel;
  NewConncetionCallack _connect_cb;
};

} //namespace net
#endif
