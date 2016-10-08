/* -*- C++ -*- */
#ifndef NET_TCP_CONNECTION_H
#define NET_TCP_CONNECTION_H
#include <memory>
#include <string>
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
                const std::string& name,
                const IpAddress& local_address,
                const IpAddress& remote_address);
  virtual ~TcpConnection();

  int get_fd() {return _fd;}
  void setCloseCallBack(CloseCallBack cb) {_close_cb = cb;}
  void setWriteCallBack(WriteCallBack cb) { _write_cb = cb;}
  void setMessageCallBack(MessageCallBack cb) {_message_cb = cb;}
  void connectEstablished() {_channel->enableRead();}
  void connectDestroied();

 private:
  void send(const char* data, int size);
  void handleRead();
  void handleWrite();
  void handleClose();

 private:
  EventLoop* _loop;
  IpAddress _local_address;
  IpAddress _remote_address;
  int _fd;
  std::string _name;
  std::unique_ptr<Channel> _channel;
  MessageCallBack _message_cb;
  WriteCompleteCallBack _write_cb;
  CloseCallBack _close_cb;
  Buffer _input_buffer;
  Buffer _output_buffer;
};

}//namespace net

#endif
