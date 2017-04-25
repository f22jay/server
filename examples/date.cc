// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/10/09 18:23:25
// Breif :

#include "date.h"
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <atomic>
#include "tcp_connection.h"
#include "event_loop.h"

namespace net {
extern int g_accept_num;
extern std::atomic<unsigned long long> g_event_num;

DateServer::DateServer(net::EventLoop* loop, net::IpAddress& address, const std::string& name):
    TcpServer(loop, address, name) {}

DateServer::~DateServer() {}

void DateServer::onMessage(const TcpConnectionPtr& conn, Buffer* buffer) {
    // common::LOG_INFO("read: [%s]", buffer->data());
  if (memcmp("exit", buffer->data(), 4) == 0) {
      conn->shutdown();
      return;
    }
    // char buf[100];
    // time_t now;
    // time(&now);
    // int size = snprintf(buf, 100, "%s", ctime(&now));
    // conn->send(buf, size);
    conn->send("world", 5);
  }

}  // net

void print_data(int sig) {
  common::LOG_FATAL("[g_accept_num:%d], [g_event_num:%d]", net::g_accept_num, net::g_event_num.load());
}

int main(int argc, char *argv[])
{
  net::IpAddress address(net::kServerIp, net::kServerPort);
  net::EventLoop* loop = new net::EventLoop();
  std::string name = "date";
  net::DateServer server(loop, address, name);
  signal(SIGUSR1, print_data);
  server.start();
  return 0;
}
