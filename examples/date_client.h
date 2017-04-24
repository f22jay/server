//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2017/04/24 12:40:07
// Breif :
#ifndef EXAMPLE_DATE_CLIENT_H
#define EXAMPLE_DATE_CLIENT_H
#include "tcp_client.h"
#include "function.h"
#include "log.h"
namespace net {
class DateClient
{
 public:
  DateClient(EventLoop* loop, IpAddress& address);
  virtual ~DateClient();
  void start();

    // read message and reply
  void onMessage(const TcpConnectionPtr& conn, Buffer* buffer);
  void onConnect(const TcpConnectionPtr& conn);

 private:
  TcpClient _client;
};

}  // net


#endif
