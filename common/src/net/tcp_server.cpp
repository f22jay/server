#include "net/tcp_server.h"
#include <boost/bind.hpp>
#include "net/tcp_connection.h"
#include "net/channel.h"
#include "net/socket.h"
#include "net/event_loop.h"

namespace net {

TcpServer::TcpServer(
    EventLoop *loop,
    IpAddress &listen_address,
    string &name): _accept(loop, listen_address), _name(name) {
  _accept.setConnectionCallBack(&boost::bind(TcpServer::newConnection, this, _1, _2));
}
TcpServer::~TcpServer() {
  //todo
}
void TcpServer::newConnection(int fd, IpAddress &peer_address) {
  IpAddress local_address;
  socket::getLocalAddr(&local_address);
  TcpConnectionPtr conn = new TcpConnection(_loop, fd, local_address, peer_address);
  _tcp_connections.insert(std::make_pair(fd, conn));
  conn->setMessageCallBack(_message_cb);
  conn->setCloseCallBack(boost::bind(&TcpServer::removeTcpConnection, this, _1));
  coon->setWriteCallBack(_write_cb);

  LOG_INFO("connection fd[%d], peer[%s]\n", fd, peeraddr.toIpPortStr());
  //activate connection
  coon->connectEstablished();
}

void TcpServer::removeTcpConnection(TcpConnectionPtr &conn) {
  map::iterator it = _tcp_connections.find(conn.get_fd());
  if (it == _tcp_connections.end()) {
    LOG_INFO("remove connection, fd [%d]null", conn->get_fd());
    return;
  }
  _tcp_connections.erase(it);
  LOG_INFO("remove connection, fd[%d]", connn->get_fd());
}
}//namespace net









