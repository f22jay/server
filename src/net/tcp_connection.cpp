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
  // _channel->setReadCallBack(std::bind(&TcpConnection::handleRead, shared_from_this()));
  // _channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, shared_from_this()));
  // _channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, shared_from_this()));
  // is error, constructor cannot use shared_from_this

  // _channel->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
  // _channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
  // _channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
}

TcpConnection::~TcpConnection() {}

void TcpConnection::init_callback() {
  // this cannot be shared_from_this, ohterwise shared_ptr use_count > 2,
  // tcpconnection can not be destructed
  _channel->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
  _channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, this));
  _channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, this));
}

void TcpConnection::handleRead() {
  // int errno;
  _input_buffer.clear();
  ssize_t size = _input_buffer.readFd(_sock->get_fd(), &errno);
  // common::LOG_DEBUG("read size [%d]", size);
  if (size < 0) {
    common::LOG_INFO("read error [%s]", strerror(errno));
    // common::LOG_INFO("read error ");
    //todo strerrro
    handleClose();
  }
  _message_cb(shared_from_this(), &_input_buffer);
}

void TcpConnection::handleWrite() {
  int n = Socket::write(_sock->get_fd(), _output_buffer.data(), _output_buffer.readableSize());
  // common::LOG_DEBUG("write size [%d]", n);
  if (n > 0) {
    _output_buffer.retrive(n);
    if (_output_buffer.readableSize() == 0) {
      //todo
      _channel->disableWrite();
      _write_cb(shared_from_this());
    } else {
      common::LOG_INFO("need write more");
    }
  } else {
    common::LOG_INFO("write error");
  }
}

void TcpConnection::connectDestroied() {
  // _channel->disableAll();
  _channel->remove();
}

void TcpConnection::handleClose() {
  // common::LOG_DEBUG("close tcpconnection fd[%d]", get_fd());
  _channel->remove();
  _close_cb(shared_from_this());
}

void TcpConnection::send(const char *data, int size) {
  ssize_t wrote = 0;
  if (! _channel->isWriting() && _output_buffer.readableSize() == 0) {
    wrote = Socket::write(_sock->get_fd(), data, size);
    if (wrote == size) {
      if (_write_cb) {
        _write_cb(shared_from_this());
      }
      return;
    }
  }
  wrote = wrote < 0 ? 0 : wrote;
  _output_buffer.append(data + wrote, size - wrote);
  _channel->enableWrite();

  return;
}
}//namespace net
