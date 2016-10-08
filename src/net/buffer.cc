// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/08 19:41:19
// Breif :
#include "buffer.h"
#include <sys/uio.h>
#include <error.h>

namespace net {
ssize_t Buffer::readFd(int fd, int* savedErrno) {
  char extrabuf[65536];
  struct iovec vec[2];
  size_t writable = writeableSize();
  vec[0].iov_base = begin() + _write_index;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  ssize_t n = readv(fd, vec, 2);
  if (n < 0) {
    *savedErrno = errno;
  } else if (static_cast<size_t>(n) <= writable) {
    _write_index += n;
  } else {
    _write_index = _buffer.size();
    append(extrabuf, n - writable);
  }
  // _buffer[_write_index++] = '\0';
  return n;
}
}
