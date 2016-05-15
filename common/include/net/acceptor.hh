// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : moon (moon@baidu.com)
// 
// Breif :listen port, accept connect request 

#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <memory>
namespace net {
class EventLoop;
class Channel;
class IpAddress;
class Socket;

class Acceptor {
 public:
  Acceptor(EventLoop *loop, IpAddress &address);
  virtual ~Acceptor();

 private:
  typedef void (* NewConncetionCallack)(int, IpAddress &);
  void handleRead();
  void setConnectionCallBack(NewConncetionCallack cb) {_connect_cb = cb;}
  Socket _sock;
  EventLoop *_loop;
  std::unique_ptr<Channel> _channel;
  NewConncetionCallack _connect_cb;
};

} //namespace net
















