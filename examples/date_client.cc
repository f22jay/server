//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2017/04/24 12:51:14
// Breif :

#include "date_client.h"
#include <signal.h>
#include <vector>
#include <memory>
#include "date.h"
#include "tcp_connection.h"
#include "event_loop.h"
#include "event_thread.h"

namespace net {
EventLoopThreadPool* eventPool = NULL;
DateClient::DateClient(EventLoop* loop, IpAddress& address):_client(loop, address) {}

DateClient::~DateClient() {}

void DateClient::onMessage(const TcpConnectionPtr& conn, Buffer* buffer) {
  common::LOG_INFO("read: [%s]", buffer->data());
  if (memcmp("exit", buffer->data(), 4) == 0) {
    conn->shutdown();
    return;
  }

  conn->send("h", 1);
}

void DateClient::onConnect(const TcpConnectionPtr& conn) {
  common::LOG_INFO("connect: fd[%d]", conn->get_fd());
  conn->send("h", 1);
}

void DateClient::start() {
  _client.setMessageCallBack(std::bind(&DateClient::onMessage,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
  _client.setConnectionCallBack(std::bind(&DateClient::onConnect,
                                          this,
                                          std::placeholders::_1));
  _client.start();
}

}  // net

void exit(int sig) {
  common::LOG_FATAL("exit \n");
  net::eventPool->Stop();
}

int main(int argc, char *argv[])
{
  net::IpAddress address(net::kServerIp, net::kServerPort);
  net::eventPool = new net::EventLoopThreadPool(4);
  std::vector<std::shared_ptr<net::DateClient> > clients;
  int nums = 2000;
  for (int i = 0; i < nums; ++i) {
    std::shared_ptr<net::DateClient> client_ptr(new net::DateClient(net::eventPool->getLoop(), address));
    clients.push_back(client_ptr);
    client_ptr->start();
  }
  // net::DateClient client(loop, address);
  // client.start();
  signal(SIGINT, exit);
  net::eventPool->Start();
  net::eventPool->Wait();
  common::LOG_FATAL("main exit");
  delete net::eventPool;
  return 0;
}
