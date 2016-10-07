/* -*- C++ -*- */

#ifndef NET_SOCKET_H
#define NET_SOCKET_H
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory.h>
#include <string>
#include <string.h>
#include <log.h>

namespace net {
const int kMaxCon = 10000;
class IpAddress {
 public:
  friend class Socket;
  IpAddress() {}
  IpAddress(const char *ip, int port) {
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    ::inet_pton(AF_INET, ip, &_address.sin_addr);
    _address.sin_port = htons(port);
  }

  IpAddress(struct sockaddr_in &address) {
    memcpy(&_address, &address, sizeof(sockaddr_in));
  }

  std::string toIpPortStr() {
    char buf[64];
    ::inet_ntop(AF_INET, &_address.sin_addr, buf, sizeof(buf));
    buf[strlen(buf) + 1] = ':';
    sprintf(buf + strlen(buf) + 1, "%u", ntohs(_address.sin_port));
    return buf;
  }

  virtual ~IpAddress() {}

 private:
  struct sockaddr_in _address;
};

class Socket {
 public:
  Socket() {
    _fd = socket(PF_INET, SOCK_STREAM, 0);
  }

  virtual ~Socket() {}
  //listen request
  bool listen() {
   return ::listen(_fd, kMaxCon) != -1;
  }
  //accept connection, block
  int accept(IpAddress &address) {
    socklen_t len = sizeof(address._address);
    int connec_fd = ::accept(_fd, (struct sockaddr*)&address._address, &len);
    if (connec_fd < 0) {
      common::LOG_INFO("accept connect error, listen fd[%d]", _fd);
      return -1;
    }
    return connec_fd;
  }
  //bind ip port
  bool bind(IpAddress &address) {
    return ::bind(_fd, (struct sockaddr*)&address._address, sizeof(address._address)) != -1;
  }
  int get_fd() {return _fd;}

  static int write(int fd, const char *data, int size) {
      return ::write(fd, data, size);
  }

  static void getLocalAddr(int sockfd, IpAddress &address) {
      struct sockaddr_in &localaddr = address._address;
      bzero(&localaddr, sizeof localaddr);
      socklen_t addrlen = sizeof(localaddr);
      if (::getsockname(sockfd, (struct sockaddr*)(&localaddr), &addrlen) < 0) {
        common::LOG_INFO("socket::getLocalAddr error");
      }
      return;
  }

  static void  getPeerAddr(int sockfd, IpAddress &address) {
      struct sockaddr_in peeraddr = address._address;
      bzero(&peeraddr, sizeof peeraddr);
      socklen_t addrlen = sizeof(peeraddr);
      if (::getpeername(sockfd, (struct sockaddr*)(&peeraddr), &addrlen) < 0) {
          common::LOG_INFO("socket::getPeerAddr error");
      }
      return;
  }

 private:
  int _fd;
};

}// namespace common
#endif
