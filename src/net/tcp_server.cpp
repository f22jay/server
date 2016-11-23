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

TcpServer::TcpServer(
    EventLoop* loop,
    const IpAddress& listen_address,
    const string& name)
    : _accept(new Acceptor(loop,
                           std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2),  listen_address)),
      _name(name), _acceptor_loop(loop){
      // _loops(NULL){
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
  _accept->listen();
  ignore_pipe();
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
  {
    common::MutexGuard mtx_guard(&_mutex);
    _tcp_connections.insert(std::make_pair(fd, conn));
  }
  conn->setMessageCallBack(_message_cb);
  conn->setCloseCallBack(std::bind(&TcpServer::removeTcpConnection, this, std::placeholders::_1));
  conn->setWriteCallBack(_write_cb);

  common::LOG_INFO("connection fd[%d], peer[%s]\n", fd, peer_address.toIpPortStr().c_str());
  //activate connection
  _loops[loop_id].runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
  // conn->connectEstablished();
  // common::LOG_DEBUG("connection use_conut[%d]", conn.use_count());
}

void TcpServer::removeTcpConnection(const TcpConnectionPtr& conn) {
  _acceptor_loop->runInLoop(std::bind(&TcpServer::removeTcpConnectionInLoop, this, conn));
}

void TcpServer::removeTcpConnectionInLoop(const TcpConnectionPtr& conn) {
  _tcp_connections.erase(conn->get_fd());
  common::LOG_INFO("remove connection fd[%d] suc, use count[%d]", conn->get_fd(), conn.use_count());
  EventLoop* loop = conn->get_loop();
  loop->runInLoop(std::bind(&TcpConnection::connectDestroied, conn));
}
}//namespace net
