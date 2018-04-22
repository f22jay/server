// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/06 16:20:48
// Breif :

#include "tcp_connection.h"
#include <errno.h>
#include "event_loop.h"
#include "log.h"

namespace net {
using std::string;
TcpConnection::TcpConnection(
    EventLoop* loop,
    int fd,
    const IpAddress& local_address,
    const IpAddress& remote_address)
    : _loop(loop), _sock(new Socket(fd)),
      _channel(new Channel(fd, loop)),
      _local_address(local_address),
      _remote_address(remote_address) {
  _sock->setNonBlock();
  _sock->setTcpNoDelay(true);
  // _channel->setReadCallBack(std::bind(&TcpConnection::handleRead, shared_from_this()));
  // _channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, shared_from_this()));
  // _channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, shared_from_this()));
  // is error, constructor cannot use shared_from_this
}

TcpConnection::TcpConnection(EventLoop* loop, const std::shared_ptr<Socket>& sock)
    : _loop(loop), _sock(sock), _channel(new Channel(sock->get_fd(), loop)) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::init_callback() {
  // this cannot be shared_from_this, ohterwise shared_ptr use_count > 2,
  // tcpconnection can not be destructed
  _channel->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
  _channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
  _channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
}

void TcpConnection::handleRead() {
  // _input_buffer.clear();
  ssize_t size = 0;
  do {
    char buf[10240];
    // size = _input_buffer.readFd(_sock->get_fd(), &errno);
    size = read(_sock->get_fd(), buf, sizeof(buf));
    if (size > 0) {
      _input_buffer.append(buf, size);
    }
    if (size < 0 && errno != EAGAIN) {
      common::LOG_FATAL("read error [%s]", strerror(errno));
      handleClose();
      return;
    }

  } while (size > 0);
  _message_cb(shared_from_this(), &_input_buffer);
}

void TcpConnection::handleWrite() {
  int n = Socket::write(_sock->get_fd(), _output_buffer.data(), _output_buffer.readableSize());
  if (n > 0) {
    _output_buffer.retrive(n);
    if (_output_buffer.readableSize() == 0) {
      // _channel->disableWrite(); LT shoud disable
      _write_cb(shared_from_this());
      _output_buffer.clear();
    } else {
      common::LOG_DEBUG("write the last size[%lu]", _output_buffer.readableSize());
      // next loop will write the last data
    }
  } else {
    common::LOG_FATAL("write error");
  }
}

void TcpConnection::connectDestroied() {
  _channel->remove();
}

void TcpConnection::handleClose() {
  _close_cb(shared_from_this());
}

void TcpConnection::send(const char *data, int size) {
  if (data == NULL || size == 0) {
    return;
  }
  _output_buffer.append(data, size);
  _loop->runInLoop(std::bind(&TcpConnection::sendInLoop, shared_from_this()));
  return;
}

void TcpConnection::sendInLoop() {
  if (_output_buffer.readableSize() == 0) {
    return;
  }
  ssize_t wrote = 0;
  do {
    wrote = Socket::write(_sock->get_fd(),
                          _output_buffer.data(),
                          _output_buffer.readableSize());
    if (wrote >= 0) {
      _output_buffer.retrive(wrote);
    } else if (wrote == -1 && errno == EAGAIN) {
      break;
    } else {
      common::LOG_FATAL("write fd[%u], error", _sock->get_fd());
      return;
    }
  } while (_output_buffer.readableSize() > 0);
  if (_write_cb) {
    _write_cb(shared_from_this());
  }
  return;
}
}//namespace net
