/* -*- C++ -*-*/
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/08 17:43:12
// Breif :

#ifndef NET_SOCKET_H
#define NET_SOCKET_H
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <memory.h>
#include <string>
#include <string.h>
#include <log.h>

namespace net {
const int kMaxCon = 20000;
class IpAddress {
 public:
  friend class Socket;
  IpAddress() {}

  IpAddress(int port) {
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = htonl(INADDR_ANY);
    _address.sin_port = htons(port);
  }

  IpAddress(const char* ip, int port) {
    memset(&_address, 0, sizeof(struct sockaddr_in));
    _address.sin_family = AF_INET;
    ::inet_pton(AF_INET, ip,& _address.sin_addr);
    _address.sin_port = htons(port);
  }

  IpAddress(struct sockaddr_in& address) {
    memcpy(&_address,& address, sizeof(sockaddr_in));
  }

  std::string toIpPortStr() {
    char buf[64];
    ::inet_ntop(AF_INET,& _address.sin_addr, buf, sizeof(buf));
    int len = strlen(buf);
    buf[len] = ':';
    sprintf(buf + len + 1, "%d", ntohs(_address.sin_port));
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

  Socket(int fd) {
    _fd = fd;
  }

  virtual ~Socket() {
    ::close(_fd);
    common::LOG_DEBUG("socket desconsturct close [%d]", _fd);
  }

  bool connect(IpAddress& address) {
    int ret = ::connect(_fd, (struct sockaddr*)&address._address, sizeof(address._address));
    // setNonBlock();
    setTcpNoDelay(true);
    return ret == 0;
  }

  //listen request
  bool listen() {
   return ::listen(_fd, kMaxCon) != -1;
  }
  //accept connection, block
  int accept(IpAddress& address) {
    socklen_t len = sizeof(address._address);
    int connec_fd = ::accept(_fd, (struct sockaddr*)&address._address,& len);
    if (connec_fd < 0) {
      common::LOG_FATAL("accept connect error, listen fd[%d], [%s]", _fd, strerror(errno));
      return -1;
    }
    return connec_fd;
  }
  //bind ip port
  bool bind(const IpAddress& address) {
    return ::bind(_fd, (struct sockaddr*)&address._address, sizeof(address._address)) != -1;
  }

  int get_fd() {return _fd;}

  void setNonBlock() {
    int flags = ::fcntl(_fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    int ret = ::fcntl(_fd, F_SETFL, flags);
  }

  void setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY,
                 &optval, static_cast<socklen_t>(sizeof optval));
  }

  void setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof optval));
  }

  void setAddrReuse(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR,
                 &optval, static_cast<socklen_t>(sizeof optval));
  }

  static int shutdown(int fd) {
    if (::shutdown(fd, SHUT_WR) < 0) {
      common::LOG_FATAL("shutdown fd[%d] error", fd);
      return -1;
    }
    common::LOG_DEBUG("shutdown fd[%d] error", fd);
    return 0;
  }

  static int write(int fd, const char* data, int size) {
      return ::write(fd, data, size);
  }

  static void getLocalAddr(int sockfd, IpAddress& address) {
      struct sockaddr_in& localaddr = address._address;
      bzero(&localaddr, sizeof localaddr);
      socklen_t addrlen = sizeof(localaddr);
      if (::getsockname(sockfd, (struct sockaddr*)(&localaddr),& addrlen) < 0) {
        common::LOG_INFO("socket::getLocalAddr error");
      }
      return;
  }

  static void getPeerAddr(int sockfd, IpAddress& address) {
      struct sockaddr_in peeraddr = address._address;
      bzero(&peeraddr, sizeof peeraddr);
      socklen_t addrlen = sizeof(peeraddr);
      if (::getpeername(sockfd, (struct sockaddr*)(&peeraddr),& addrlen) < 0) {
          common::LOG_INFO("socket::getPeerAddr error");
      }
      return;
  }

 private:
  int _fd;
};

}// namespace common
#endif
