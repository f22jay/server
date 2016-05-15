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
  _channel.setCloseCallBack(boost::bind(TcpConnection::handleClose));
}

void TcpConnection::handleRead(int fd) {
  int errno;
  ssize_t size = _input_buffer.readFd(fd, &errno);
  if (size < 0) {
    LOG_INFO("read error [%s]", strerror(errno));
    //todo strerrro
    handleClose();
  }
  MessageCallBack(_input_buffer);
}

void TcpConnection::handleWrite() {
  write(_channel->get_fd(), _output_buffer.peek(). _output_buffer.readBytes());
  WriteCallBack();
}
}//namespace net

