/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/10/08 17:43:26
// Breif :

#ifndef NET_TCP_CONNECTION_H
#define NET_TCP_CONNECTION_H
#include <memory>
#include "function.h"
#include "channel.h"
#include "socket.h"
#include "buffer.h"

namespace net {
class EventLoop;
class TcpConnection: public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(EventLoop* loop,
                int fd,
                const IpAddress& local_address,
                const IpAddress& remote_address);
  TcpConnection(EventLoop* loop, int fd);
  virtual ~TcpConnection();

  int get_fd() {return _sock->get_fd();}
  EventLoop* get_loop() {return _loop;}
  void init_callback();
  int shutdown() {return Socket::shutdown(get_fd());}
  void setCloseCallBack(CloseCallBack cb) {_close_cb = cb;}
  void setWriteCallBack(WriteCallBack cb) { _write_cb = cb;}
  void setMessageCallBack(MessageCallBack cb) {_message_cb = cb;}

  // activate channel
  void connectEstablished() {_channel->enableRead();}
  // remove channel
  void connectDestroied();

  void send(const char* data, int size);

 private:
  void handleRead();
  void handleWrite();
  void handleClose();

 private:
  EventLoop* _loop;
  IpAddress _local_address;
  IpAddress _remote_address;
  std::unique_ptr<Socket> _sock;;
  std::unique_ptr<Channel> _channel;
  MessageCallBack _message_cb;
  WriteCompleteCallBack _write_cb;
  CloseCallBack _close_cb;
  Buffer _input_buffer;
  Buffer _output_buffer;
};

}//namespace net

#endif
