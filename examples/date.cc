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
net::DateServer* server = NULL;
DateServer::DateServer(net::IpAddress& address, const std::string& name, int loop_num):
    TcpServer(address, name, loop_num) {}

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
    conn->send("w", 1);
  }

}  // net

void print_data(int sig) {
  common::LOG_FATAL("[current_connection_nums:%lu], [sum_connection_nums:%d], [sum_event_nums:%llu]",
                    net::server->size(),
                    net::g_accept_num,
                    net::g_event_num.load());
}

int main(int argc, char *argv[])
{
  net::IpAddress address(net::kServerIp, net::kServerPort);
  // net::EventLoop* loop = new net::EventLoop();
  std::string name = "date";
  net::server = new net::DateServer(address, name, 4);
  signal(SIGUSR1, print_data);
  net::server->start();
  return 0;
}
