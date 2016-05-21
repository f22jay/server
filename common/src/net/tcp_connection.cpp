#include "net/tcp_connection.h"
#include <boost/bind.hpp>
#include "net/channel.h"
#include "net/socket.h"
#include "net/event_loop.h"
#include "net/buffer.h"

namespace net {
TcpConnection::TcpConnection(
    EventLoop *loop,
    int fd,
    const string &name,
    const IpAddress &local_address,
    const IpAddress &remote_address)
    : _loop(loop), _fd(fd), _name(name), _channel(loop, fd) {
  _channel.setReadCallBack(boost::bind(TcpConnection::handleRead, this, _1));
  _channel.setWriteCallBack(boost::bind(TcpConnection::handleWrite, this));
  _channel.setCloseCallBack(boost::bind(TcpConnection::handleClose, this));
}

void TcpConnection::handleRead(int fd) {
  int errno;
  ssize_t size = _input_buffer.readFd(fd, &errno);
  if (size < 0) {
    LOG_INFO("read error [%s]", strerror(errno));
    //todo strerrro
    handleClose();
  }
  MessageCallBack(this, _input_buffer);
}

void TcpConnection::handleWrite() {
  int n = Socket::write(_channel->get_fd(), _output_buffer.data(). _output_buffer.readBytes());
  if (n > 0) {
    _output_buffer.retrive(n);
    if (_output_buffer.readBytes() == 0) {
      _channel.disableWrite();
      WriteCallBack(this);
    } else {
      LOG_INFO("need write more");
    }
  } else {
    LOG_INFO("write error");
  }
}

void TcpConnection::connectDestroied() {
  _channel.disableAll();
  _channel.remove();
}

void TcpConnection::handleClose() {
  _close_cb(this); 
}

void TcpConnection::send(const char *data, int size) {
  ssize_t wrote = 0;
  if (! _channel.isWriting() && _output_buffer.readableSize() == 0) {
    wrote = Socket::write(_channel->get_fd(), data, size);
    if (worte == size) {
      _writeCallBack(this);
      return;
    }
  }
  wrote = wrote < 0 ? 0 : wrote;
  _output_buffer.append(data + worte, size - wrote);
  _channel.enableWrite();
  
  return;
}
}//namespace net



















