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
#include "event_loop.h"
#include "function.h"
#include "mutex.h"

namespace net {
class Acceptor;
class TcpConnection;
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
  void removeTcpConnectionInLoop(const TcpConnectionPtr& conn);
  void ignore_pipe();

 private:
  IpAddress _listen_address;
  std::shared_ptr<Acceptor> _accept;
  std::unique_ptr<EventLoop[]> _loops;
  EventLoop* _acceptor_loop;
  map<int, TcpConnectionPtr> _tcp_connections;
  NewConncetionCallack _connect_cb;
  MessageCallBack _message_cb;
  WriteCallBack _write_cb;
  string _name;
  int64_t _conn_num;
  const int _eventloop_num = 4;
  common::Mutex _mutex;

};
} //namespace net

#endif
