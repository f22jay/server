/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/07 19:13:43
// Breif :

#ifndef NET_BUFFER_H
#define NET_BUFFER_H
#include <vector>
#include <algorithm>
#include "log.h"

namespace net{
class Buffer {
 public:
  Buffer():_read_index(0),
           _write_index(0),
           _buffer(kBufferSize) {}
  ~Buffer() {common::LOG_DEBUG("read_index[%d], write_index[%d], buffer size[%d]",
                               _read_index, _write_index, _buffer.size());}

  void append(const char *data, int length) {
    if (writeableSize() < length) {
      makeSpace(length);
    }
    std::copy(data, data + length, writeBegin());
    _write_index += length;
  }

  void retrive(int n) {
    _read_index += n;
  }

  char* writeBegin() {
    return begin() + _write_index;
  }

  size_t readableSize() const {
    return _write_index - _read_index;
  }

  size_t writeableSize() const {
    return kBufferSize - _write_index;
  }

  char* begin() {
    return &_buffer.front();
  }

  char* data() {
    return begin() + _read_index;
  }

  int size() {
    return _buffer.size();
  }

  void clear() {
    _read_index = _write_index = 0;
    // std::vector<char> buf(kBufferSize);
    // _buffer.swap(buf);
  }

  void makeSpace(size_t length) {
    //buffer not used size < length
    if (size() - readableSize()  < length) {
      _buffer.resize(length + size());
    } else { //just move
      std::copy(data(), data() + readableSize(), begin());
      _write_index -= _read_index;
      _read_index = 0;
    }
  }
  ssize_t readFd(int fd, int* savedErrno);
 private:
  std::vector<char> _buffer;
  static const size_t kBufferSize = 10240;
  size_t _read_index;
  size_t _write_index;
};

}//namespace net

#endif  // BUFFER_H
