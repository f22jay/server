#ifndef _SOCKET_H__107040068846139311_
#define _SOCKET_H__107040068846139311_
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <string>

namespace net {
const int kMaxCon = 10000;
class IpAddress
{
 public:
  friend class Socket;
  IpAddress(const char *ip, int port) {
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&_address.sin_addr);
    _address.sin_port = htons(port);
  }

  IpAddress(struct sockaddr_in &address) {
    
  }

  std::string toIpPortStr() {
    char buf[64];
    //todo
    return std::string();
  }
  
  virtual ~IpAddress();

 private:
  struct sockaddr_in _address;
};

class Socket
{
 public:
  Socket() {
    _fd = socket(PF_INET, SOCK_STREAM, 0);
  }
  virtual ~Socket();
  //listen request
  bool listen() {
   return ::listen(_fd, kMaxCon) != -1;
  }
  //accept connection, block
  int accept(IpAddress &address) {
    int len;
    int connec_fd = ::accept(_fd, (struct sockaddr *)&address._address, &len);
    if (connec_fd < 0) {
      LOG_INFO("accept connect error, listen fd[%d]", _fd);
      return -1;
    }
    return connec_fd;
  }
  //bind ip port
  bool bind(IpAddress &address) {
    return ::bind(_fd, (struct sockaddr *) address._address), sizeof(address._address) != -1;
  }
  int get_fd() {return _fd;}

  static void write(int fd, const char *data, int size) {
      return ::write(fd, data, size);
  }

  static void getLocalAddr(int sockfd, IpAddress &address) {
      struct sockaddr_in &localaddr = address._address;
      bzero(&localaddr, sizeof localaddr);
      socklen_t addrlen = sizeof(localaddr);
      if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0) {
        LOG_INFO("socket::getLocalAddr");
      }
      return;
  }

  static void  getPeerAddr(int sockfd, IpAddress &address) {
      struct sockaddr_in peeraddr = address._address;
      bzero(&peeraddr, sizeof peeraddr);
      socklen_t addrlen = sizeof(peeraddr);
      if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0) {
          LOG_INFO("socket::getPeerAddr");
      }
      return;
  }
  
 private:
  int _fd;
};

}


#endif



