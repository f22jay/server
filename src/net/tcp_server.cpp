#include "tcp_server.h"
#include <functional>
#include "tcp_connection.h"
#include "channel.h"
#include "acceptor.h"
#include "socket.h"
#include "event_loop.h"

namespace net {

TcpServer::TcpServer(
    EventLoop* loop,
    const IpAddress& listen_address,
    const string& name)
    : _accept(new Acceptor(loop, std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2), listen_address)), _name(name), _loop(loop) {
}

TcpServer::~TcpServer() {
  //todo
  for(const auto& it: _tcp_connections) {
    it.second->connectDestroied();
  }
}

void TcpServer::start() {
  _accept->listen();
  _loop->poll();
}

void TcpServer::newConnection(int fd, IpAddress& peer_address) {
  IpAddress local_address;
  Socket::getLocalAddr(fd, local_address);
  //todo name
  TcpConnectionPtr conn(new TcpConnection(_loop, fd, local_address, peer_address));
  conn->init_callback();
  _tcp_connections.insert(std::make_pair(fd, conn));
  conn->setMessageCallBack(_message_cb);
  conn->setCloseCallBack(std::bind(&TcpServer::removeTcpConnection, this, std::placeholders::_1));
  conn->setWriteCallBack(_write_cb);

  common::LOG_INFO("connection fd[%d], peer[%s]\n", fd, peer_address.toIpPortStr().c_str());
  //activate connection
  conn->connectEstablished();
  // common::LOG_DEBUG("connection use_conut[%d]", conn.use_count());
}

void TcpServer::removeTcpConnection(const TcpConnectionPtr& conn) {
  auto it = _tcp_connections.find(conn->get_fd());
  if (it == _tcp_connections.end()) {
      common::LOG_INFO("cannot find connection fd [%d] ", conn->get_fd());
      return;
  }
  _tcp_connections.erase(it);
  common::LOG_INFO("remove connection fd[%d] suc, use count[%d]", conn->get_fd(), conn.use_count());
}
}//namespace net
