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

TcpClient::TcpClient(EventLoop* loop, const IpAddress& address): _loop(loop), _server(address) {}

TcpClient::~TcpClient() {}

void TcpClient::start() {
  _sock.connect(_server);
  _tcp_connection.reset(new TcpConnection(_loop, _sock.get_fd()));
  _tcp_connection->setMessageCallBack(_message_cb);
  _tcp_connection->setWriteCallBack(_write_cb);
  // _tcp_connection.setCloseCallBack
  _tcp_connection->init_callback();
  _tcp_connection->connectEstablished();
  _connect_cb(_tcp_connection);

}

}  // net
