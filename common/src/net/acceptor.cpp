#include "net/accpetor.h"
#include <boost/bind.h>
#include "net/socket.h"
#include "base/logging.h"

namespace net {
Acceptor::Acceptor(EventLoop *loop, NewConncetionCallack cb, IpAddress *address)
    : _loop(loop),
      _connect_callback(cb),
      _sock(address),
      _channel(new Channel(_sock.get_fd(), _loop)) {
  _sock.listen();
  _sock.bind(address);
  _channel->setReadCallBack(boost::bind(&Acceptor::handleRead, this));
  _channel->enableRead();
}

void Acceptor::handleRead() {
  IpAddress address;
  int accept_fd = _sock.accept(address);
  assert(accept_fd > 0);
  LOG_INFO("accept [%s] connect", address.toIpPort().c_str());
  _connect_callback(accept_fd);
}


}//namespace net
