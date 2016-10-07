#include "tcp_connection.h"
#include "event_loop.h"
#include "log.h"

namespace net {
using std::string;
TcpConnection::TcpConnection(
    EventLoop* loop,
    int fd,
    const string& name,
    const IpAddress& local_address,
    const IpAddress& remote_address)
    : _loop(loop), _fd(fd), _name(name), _channel(new Channel(fd, loop)) {
  _channel->setReadCallBack(std::bind(&TcpConnection::handleRead, shared_from_this()));
  _channel->setWriteCallBack(std::bind(&TcpConnection::handleWrite, shared_from_this()));
  _channel->setCloseCallBack(std::bind(&TcpConnection::handleClose, shared_from_this()));
}

void TcpConnection::handleRead() {
  int errno;
  ssize_t size = _input_buffer.readFd(_fd, &errno);
  if (size < 0) {
    common::LOG_INFO("read error [%s]", strerror(errno));
    //todo strerrro
    handleClose();
  }
  _message_cb(shared_from_this(), &_input_buffer);
}

void TcpConnection::handleWrite() {
  int n = Socket::write(_fd, _output_buffer.data(), _output_buffer.readableSize());
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
  // todo
  // _channel->disableAll();
  _channel->remove();
}

void TcpConnection::handleClose() {
  _close_cb(shared_from_this());
}

void TcpConnection::send(const char *data, int size) {
  ssize_t wrote = 0;
  if (! _channel->isWriting() && _output_buffer.readableSize() == 0) {
    wrote = Socket::write(_fd, data, size);
    if (wrote == size) {
      _write_cb(shared_from_this());
      return;
    }
  }
  wrote = wrote < 0 ? 0 : wrote;
  _output_buffer.append(data + wrote, size - wrote);
  _channel->enableWrite();

  return;
}
}//namespace net
