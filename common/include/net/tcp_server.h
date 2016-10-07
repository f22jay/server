/* -*- C++ -*-*/
#ifndef NET_TCP_SERVER_H
#define NET_TCP_SERVER_H
#include <map>
#include <memory>
#include <string>
#include "socket.h"
#include "function.h"

namespace net {
class Acceptor;
class TcpConnection;
class EventLoop;
class Buffer;
using std::string;
using std::map;
class TcpServer {
 public:
  typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
  //new connect come, do
  typedef void (* NewConncetionCallack)(int);

  TcpServer(EventLoop* loop, IpAddress& address, string& name);
  virtual ~TcpServer();

 private:
  std::unique_ptr<Acceptor> _accept;
  EventLoop* _loop;
  map<int, TcpConnectionPtr> _tcp_connections;
  NewConncetionCallack _connect_cb;
  MessageCallBack _message_cb;
  WriteCallBack _write_cb;
  string _name;
  void newConnection(int fd, IpAddress& );
  void removeTcpConnection(const TcpConnectionPtr& conn);
};
} //namespace net

#endif
