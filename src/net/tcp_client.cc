//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2017/04/24 11:04:40
// Breif :

#include "tcp_client.h"
#include <functional>
#include "tcp_connection.h"
#include "channel.h"
#include "acceptor.h"
#include "socket.h"
#include "event_loop.h"

namespace net {

TcpClient::TcpClient(EventLoop* loop, const IpAddress& address): _loop(loop),
                                                                 _server(address),
                                                                 _sock(new Socket()){}

TcpClient::~TcpClient() {common::LOG_DEBUG("tcpclient desconsturct");}

void TcpClient::close(const TcpConnectionPtr& conn) {
  common::LOG_INFO("connection fd[%d] closed", conn->get_fd());
  _loop->runInLoop(std::bind(&TcpConnection::connectDestroied, conn));
}
void TcpClient::start() {
  _sock->connect(_server);
  _tcp_connection.reset(new TcpConnection(_loop, _sock));
  _tcp_connection->setMessageCallBack(_message_cb);
  _tcp_connection->setWriteCallBack(_write_cb);
  _tcp_connection->setCloseCallBack(std::bind(&TcpClient::close, this, std::placeholders::_1));
  _tcp_connection->init_callback();
  _tcp_connection->connectEstablished();
  _connect_cb(_tcp_connection);

}

}  // net
