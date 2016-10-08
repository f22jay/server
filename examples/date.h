/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/10/09 18:23:46
// Breif :

#ifndef EXAMPLE_DATE_H
#define EXAMPLE_DATE_H

#include "tcp_server.h"
#include <time.h>
#include <stdio.h>
#include "function.h"
#include "log.h"
namespace net {
class DateServer {
 public:
  DateServer(EventLoop* loop, IpAddress& address, const std::string& name);
  virtual ~DateServer();
  void start();

  // read message and reply
  void onMessage(const TcpConnectionPtr& conn, Buffer* buffer);

 private:
  TcpServer _server;
};

}//namespace net
#endif
