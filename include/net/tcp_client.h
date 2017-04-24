//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2017/04/24 10:53:31
// Breif :

#ifndef INCLUDE_TCP_CLIENT_H
#define INCLUDE_TCP_CLIENT_H
#include <memory>
#include "socket.h"
#include "function.h"
namespace net {

class TcpConnection;
class EventLoop;
class Buffer;
class Channel;
class TcpClient
{
 public:
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

  TcpClient(EventLoop* loop, const IpAddress& address);
  virtual ~TcpClient();
  void start();
  void setMessageCallBack(MessageCallBack cb) {_message_cb = cb;}
  void setWriteCallBack(WriteCallBack cb) {_write_cb = cb;}
  void setConnectionCallBack(ConnectionCallBack cb) {_connect_cb = cb;}

 private:
  Socket _sock;
  IpAddress _server;
  EventLoop* _loop;
  TcpConnectionPtr _tcp_connection;
  ConnectionCallBack _connect_cb;
  MessageCallBack _message_cb;
  WriteCallBack _write_cb;
};



}  // net


#endif
