// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/09 15:34:53
// Breif :

#include "acceptor.h"
#include <functional>
#include "socket.h"
#include "channel.h"
#include "log.h"

namespace net {
Acceptor::Acceptor(EventLoop* loop, NewConncetionCallack cb, const IpAddress& address)
    : _loop(loop),
      _connect_cb(cb),
      _sock(new Socket()),
      _channel(new Channel(_sock->get_fd(), _loop)) {
  _sock->bind(address);
  _sock->setNonBlock();
}
Acceptor::~Acceptor() {}

void Acceptor::listen() {
  _sock->listen();
  _channel->setReadCallBack(std::bind(&Acceptor::handleRead, shared_from_this()));
  _channel->enableRead();
  common::LOG_DEBUG("acceptor listen");
}

void Acceptor::handleRead() {
  IpAddress address;
  int accept_fd = _sock->accept(address);
  if (accept_fd < 0) {
    common::LOG_FATAL("accept error");
    return;
  }
  common::LOG_DEBUG("accept [%s] connect", address.toIpPortStr().c_str());
  _connect_cb(accept_fd, address);
}

}//namespace net
