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

namespace net {
EventLoop* loop = NULL;
std::vector<std::shared_ptr<net::DateClient> > clients;
int current = 0;
DateClient::DateClient(EventLoop* loop, IpAddress& address):_client(loop, address) {}

DateClient::~DateClient() {}

void DateClient::onMessage(const TcpConnectionPtr& conn, Buffer* buffer) {
  common::LOG_INFO("read: [%s]", buffer->data());
  if (memcmp("exit", buffer->data(), 4) == 0) {
    conn->shutdown();
    return;
  }

  conn->send("hello", 5);
}

void DateClient::onConnect(const TcpConnectionPtr& conn) {
  current++;
  if (current < clients.size()) {
    clients[current]->start();
  }
  common::LOG_INFO("connect: fd[%d]", conn->get_fd());
  conn->send("hello\n", 6);
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

// void exit(int sig) {
//   common::LOG_FATAL("exit \n");
//   net::loop->stop();
// }

int main(int argc, char *argv[])
{
  net::IpAddress address(net::kServerIp, net::kServerPort);
  net::loop = new net::EventLoop();
  int nums = 5000;
  net::clients.reserve(nums);
  for (int i = 0; i < nums; ++i) {
    std::shared_ptr<net::DateClient> client_ptr(new net::DateClient(net::loop, address));
    net::clients.push_back(client_ptr);
  }

  net::clients[net::current]->start();

  // net::DateClient client(loop, address);
  // client.start();
  // signal(SIGINT, exit);
  net::loop->poll();
  common::LOG_INFO("main exit");
  delete net::loop;
  return 0;
}
