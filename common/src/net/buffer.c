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
#include "net/buffer.h"

namespace net {

ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  char extrabuf[65536];
  struct iovec vec[2];
  size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  ssize_t n = readv(fd, vec, 2);
  if (n < 0) {
    *savedErrno = errno;
  } else if (implicit_cast<size_t>(n) <= writable) {
    writerIndex_ += n;
  }
  else {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  return n;
}
}

