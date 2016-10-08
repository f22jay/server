// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/10/09 18:23:25
// Breif :

#include "date.h"
#include "string.h"
#include "tcp_connection.h"
#include "event_loop.h"

namespace net {
DateServer::DateServer(net::EventLoop* loop, net::IpAddress& address, const std::string& name):
    _server(loop, address, name) {}

DateServer::~DateServer() {}

void DateServer::onMessage(const TcpConnectionPtr& conn, Buffer* buffer) {
    common::LOG_INFO("read: [%s]", buffer->data());
    if (memcmp("exit", buffer->data(), 4) == 0) {
      conn->shutdown();
      return;
    }
    char buf[100];
    time_t now;
    time(&now);
    int size = snprintf(buf, 100, "%s", ctime(&now));
    conn->send(buf, size);
    // conn->shutdown();
  }

void DateServer::start() {
  _server.setMessageCallBack(std::bind(&DateServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
  _server.start();
}
}  // net

int main(int argc, char *argv[])
{
  net::IpAddress address("10.128.144.17", 10086);
  net::EventLoop* loop = new net::EventLoop();
  std::string name = "date";
  net::DateServer server(loop, address, name);
  server.start();
  loop->poll();
  return 0;
}
