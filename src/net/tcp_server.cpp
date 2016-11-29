#include "tcp_server.h"
#include <signal.h>
#include <functional>
#include "tcp_connection.h"
#include "channel.h"
#include "acceptor.h"
#include "socket.h"
#include "event_loop.h"
#include "thread.h"

namespace net {

TcpServer::TcpServer(EventLoop* loop, const IpAddress& listen_address, const string& name)
    : _name(name), _acceptor_loop(loop), _listen_address(listen_address){
  _conn_num = 0;
}

TcpServer::~TcpServer() {
  //todo
  for(const auto& it: _tcp_connections) {
    it.second->connectDestroied();
  }
}

void TcpServer::ignore_pipe() {
  signal(SIGPIPE, SIG_IGN);
}

void TcpServer::start() {
  _accept.reset(new Acceptor(_acceptor_loop,
                             std::bind(&TcpServer::newConnection,
                                       this, std::placeholders::_1,
                                       std::placeholders::_2),
                             _listen_address));
  ignore_pipe();
  std::vector<common::Thread> threads(_eventloop_num);
  _loops.reset(new EventLoop[_eventloop_num]);

  auto thread_run = [this] (int i) {this->_loops[i].poll();};
  // start eventloop thread
  for (int n = 0; n < _eventloop_num; n++) {
    threads[n].Start(std::bind(thread_run, n));
  }

  // start listen loop
  _accept->start();
}

void TcpServer::newConnection(int fd, IpAddress& peer_address) {
  _conn_num++;
  int loop_id = _conn_num % _eventloop_num;
  IpAddress local_address;
  Socket::getLocalAddr(fd, local_address);

  TcpConnectionPtr conn(new TcpConnection(&_loops[loop_id], fd, local_address, peer_address));
  conn->init_callback();
  _tcp_connections.insert(std::make_pair(fd, conn));
  conn->setMessageCallBack(_message_cb);
  conn->setCloseCallBack(std::bind(&TcpServer::removeTcpConnection, this, std::placeholders::_1));
  conn->setWriteCallBack(_write_cb);

  common::LOG_INFO("connection fd[%d], peer[%s]\n", fd, peer_address.toIpPortStr().c_str());
  //activate connection
  _loops[loop_id].runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeTcpConnection(const TcpConnectionPtr& conn) {
  _acceptor_loop->runInLoop(std::bind(&TcpServer::removeTcpConnectionInLoop, this, conn));
}

void TcpServer::removeTcpConnectionInLoop(const TcpConnectionPtr& conn) {
  _tcp_connections.erase(conn->get_fd());
  common::LOG_INFO("remove connection fd[%d] suc", conn->get_fd());
  EventLoop* loop = conn->get_loop();
  loop->runInLoop(std::bind(&TcpConnection::connectDestroied, conn));
}
}//namespace net
