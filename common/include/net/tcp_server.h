#ifndef _TCP_SERVER_H__537390648655438541_
#define _TCP_SERVER_H__537390648655438541_
#include <map>
#include <unique_ptr>
#include <string>

class Acceptor;
class TcpConnection;
class IpAddress;
class EventLoop;

namespace net {
using std::string;
using std::map;
class TcpServer
{
 public:
  TcpServer(EventLoop *loop, IpAddress &address, string &name);
  virtual ~TcpServer();

 private:
  typedef std::unique_ptr<TcpConnection> TcpConnectionPtr;
  //new connect come, do
  typedef void (* NewConncetionCallack)(int);
  //after read message, call back
  typedef void (* MessageCallBack) (Buffer *buf);
  typedef void (* WriteCallBack) (Buffer *buf);

  Acceptor _accept;
  EventLoop *_loop;
  map<int, TcpConnectionPtr> _tcp_connections;
  NewConncetionCallack _connect_cb;
  MessageCallBack _message_cb;
  WriteCallBack _write_cb;
  string _name;
  void newConnection(int fd, IpAddress &);
  void removeTcpConnection(string &);

}

#endif
