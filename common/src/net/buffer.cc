/***************************************************************************
 *
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file src/net/buffer.c
 * @author zhangfangjie(zhangfangjie@baidu.com)
 * @date 2016/03/08 19:41:19
 * @version $Revision$
 * @brief
 *
 **/
#include <sys/uio.h>
#include <error.h>
#include "buffer.h"

namespace net {

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
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
  }
  else {
    _write_index = _buffer.size();
    append(extrabuf, n - writable);
  }
  return n;
}
}
