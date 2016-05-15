#ifndef _TCP_CONNECTION_H_1607244904433259219_
#define _TCP_CONNECTION_H_1607244904433259219_
#include <memory>
class Channel;
class IpAddress;
class Buffer;
class EventLoop;

namespace net {
class TcpConnection
{
 public:
  TcpConnection(EventLoop loop, int fd, const string &name,const IpAddress &local_address, const IpAddress &remote_address);
  virtual ~TcpConnection();

 private:
  EventLoop _loop;
  IpAddress _local_address;
  IpAddress _remote_address;
  std::unique_ptr<Channel> _channel;
  MessageCallBack _message_cb;
  WriteCallBack _write_cb;
  Buffer _input_buffer;
  Buffer _output_buffer;

  void send();
  void handleRead(int fd);
  void handleWrite();
  void handleClose();
  typedef void (* MessageCallBack) (Buffer *buf);
  typedef void (* WriteCallBack) (Buffer *buf);

    
};

}//namespace net

#endif
