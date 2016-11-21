#include "tcp_server.h"
#include <functional>
#include "tcp_connection.h"
#include "channel.h"
#include "acceptor.h"
#include "socket.h"
#include "event_loop.h"
#include "thread.h"

namespace net {

TcpServer::TcpServer(
    EventLoop* loop,
    const IpAddress& listen_address,
    const string& name)
    : _accept(new Acceptor(loop,
                           std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2),  listen_address)),
      _name(name), _acceptor_loop(loop) {
  _conn_num = 0;
}

TcpServer::~TcpServer() {
  //todo
  for(const auto& it: _tcp_connections) {
    it.second->connectDestroied();
  }
}

void TcpServer::start() {
  _accept->listen();
  std::vector<common::Thread> threads(_eventloop_num);
  _loops.reset(new EventLoop[_eventloop_num]);

  // start _eventloop_num threads
  for (int n = 0; n < _eventloop_num; n++) {
    threads[n].Start(std::bind(&TcpServer::event_loop_run, this, n));
  }
  _acceptor_loop->poll();
}

void TcpServer::event_loop_run(int i) {
  _loops[i].poll();
}
void TcpServer::newConnection(int fd, IpAddress& peer_address) {
  _conn_num++;
  int loop_id = _conn_num % _eventloop_num;
  IpAddress local_address;
  Socket::getLocalAddr(fd, local_address);
  TcpConnectionPtr conn(new TcpConnection(&_loops[loop_id], fd, local_address, peer_address));
  conn->init_callback();
  _mutex.Lock();
  _tcp_connections.insert(std::make_pair(fd, conn));
  _mutex.UnLock();
  conn->setMessageCallBack(_message_cb);
  conn->setCloseCallBack(std::bind(&TcpServer::removeTcpConnection, this, std::placeholders::_1));
  conn->setWriteCallBack(_write_cb);

  common::LOG_INFO("connection fd[%d], peer[%s]\n", fd, peer_address.toIpPortStr().c_str());
  //activate connection
  conn->connectEstablished();
  // common::LOG_DEBUG("connection use_conut[%d]", conn.use_count());
}

void TcpServer::removeTcpConnection(const TcpConnectionPtr& conn) {
  common::Mutex mutex;
  _mutex.Lock();
  auto it = _tcp_connections.find(conn->get_fd());
  if (it == _tcp_connections.end()) {
      common::LOG_INFO("cannot find connection fd [%d] ", conn->get_fd());
      return;
  }
  _tcp_connections.erase(it);
  _mutex.UnLock();
  common::LOG_INFO("remove connection fd[%d] suc, use count[%d]", conn->get_fd(), conn.use_count());
}
}//namespace net
