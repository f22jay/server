#ifndef _TCP_CONNECTION_H_1607244904433259219_
#define _TCP_CONNECTION_H_1607244904433259219_
#include <memory>
#include <string>
#include "net/function.h"

namespace net {
class Channel;
class IpAddress;
class Buffer;
class EventLoop;

class TcpConnection
{
 public:
  TcpConnection(EventLoop loop, int fd, const std::string &name,const IpAddress &local_address, const IpAddress &remote_address);
  virtual ~TcpConnection();

 private:
  
  EventLoop _loop;
  IpAddress _local_address;
  IpAddress _remote_address;
  int _fd;
  std::unique_ptr<Channel> _channel;
  MessageCallBack _message_cb;
  WriteCompleteCallBack _write_cb;
  CloseCallBack _close_cb;
  Buffer _input_buffer;
  Buffer _output_buffer;

  int get_fd() {return _fd;}
  void setCloseCallBack(CloseCallBack cb) {_close_cb = cb;}
  void setWriteCallBack(WriteCallBack cb) { _write_cb = cb;}
  void setMessageCallBack(MessageCallBack cb) {_message_cb = cb;}

  void connectEstablished() {_channel->enableRead();}
  void connectDestroied();
  void send(const char* data, int size);
  void handleRead(int fd);
  void handleWrite();
  void handleClose();
    
};

}//namespace net

#endif










