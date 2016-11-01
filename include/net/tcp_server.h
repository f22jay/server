/* -*- C++ -*-*/
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/10/08 15:39:56
// Breif :

#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H
#include <map>
#include <memory>
#include <string>
#include "socket.h"
#include "function.h"

namespace net {
class Acceptor;
class TcpConnection;
class EventLoop;
class Buffer;
using std::string;
using std::map;

class TcpServer {
 public:
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
  //new connect come, do
  typedef void (* NewConncetionCallack)(int);

  TcpServer(EventLoop* loop, const IpAddress& address, const string& name);
  virtual ~TcpServer();
  void start();
  // void stop();
  void setMessageCallBack(MessageCallBack cb) {_message_cb = cb;}
  void setWriteCallBack(WriteCallBack cb) {_write_cb = cb;}
 private:
  void newConnection(int fd, IpAddress& );
  void removeTcpConnection(const TcpConnectionPtr& conn);

 private:
  std::shared_ptr<Acceptor> _accept;
  EventLoop* _loop;
  map<int, TcpConnectionPtr> _tcp_connections;
  NewConncetionCallack _connect_cb;
  MessageCallBack _message_cb;
  WriteCallBack _write_cb;
  string _name;
  int64_t _conn_num;
  int _eventloop_num;
};
} //namespace net

#endif
