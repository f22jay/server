// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/09 15:34:53
// Breif :

#include "acceptor.h"
#include <functional>
#include <assert.h>
#include "socket.h"
#include "channel.h"
#include "event_loop.h"
#include "log.h"

namespace net {
int g_accept_num = 0;

Acceptor::Acceptor(EventLoop* loop, NewConncetionCallack cb, const IpAddress& address)
    : _loop(loop),
      _connect_cb(cb),
      _sock(new Socket()),
      _channel(new Channel(_sock->get_fd(), _loop)) {
  assert(_sock->bind(address));
  _sock->setNonBlock();
}
Acceptor::~Acceptor() {}

void Acceptor::start() {
  _sock->listen();
  _channel->setReadCallBack(std::bind(&Acceptor::handleRead, this));
  _channel->enableRead();
  common::LOG_DEBUG("acceptor listen");
  _loop->poll();
}

void Acceptor::handleRead() {
  IpAddress address;
  int accept_fd = _sock->accept(address);
  if (accept_fd < 0) {
    common::LOG_FATAL("accept error");
    return;
  }
  _connect_cb(accept_fd, address);
  g_accept_num++;
}

void Acceptor::handleClose() {
  common::LOG_WARNING("NOTICE: Acceptor closed!!!!!!!");
  _loop->stop();
}

}//namespace net
