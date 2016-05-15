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
  
}

void TcpServer::removeTcpConnection(string &name) {
  map::iterator it = _tcp_connections.find(name);
  if (it == _tcp_connections.end()) {
    LOG_INFO("remove connection, find [%s]null", name.c_str());
    return;
  }

  _tcp_connections.erase(it);
}//namespace net
