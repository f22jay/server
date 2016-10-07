/* -*- C++ -*- */
/***************************************************************************
 *
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file buffer.h
 * @author zhangfangjie(zhangfangjie@baidu.com)
 * @date 2016/03/07 19:13:43
 * @version $Revision$
 * @brief
 *
 **/
#ifndef BUFFER_H
#define BUFFER_H
#include <vector>
#include <algorithm>

namespace net{
  class Buffer {
 public:
 Buffer():_read_index(0),
        _write_index(0),
        _buffer(kBufferSize) {}

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
      return kBufferSize - _read_index;
    }

    char* begin() {
      return &_buffer.front();
    }

    char* data() {
      return begin() + _read_index;
    }

    void clear() {
      _read_index = _write_index = 0;
    }

    void makeSpace(size_t length) {
      if (writeableSize() + _read_index < length) {
        _buffer.resize(length - writeableSize());
      } else {
        std::copy(data(), data() + readableSize(), begin());
        _write_index -= _read_index;
        _read_index = 0;
      }
    }
    ssize_t readFd(int fd, int* savedErrno);
 private:
    std::vector<char> _buffer;
    static const size_t kBufferSize = 1024;
    size_t _read_index;
    size_t _write_index;
  };

}//namespace net

#endif  // BUFFER_H
