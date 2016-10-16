// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/10/17 14:56:08
// Breif :

#include <iostream>
#include <buffer.h>
#include <string.h>
#include <assert.h>

void TEST_apend() {
  net::Buffer buffer;
  assert(buffer.readableSize() == 0);
  assert(buffer.writeableSize() == 1024);

  char buf[10000] = "0";
  memset(buf, 0, 10000);
  memset(buf, '-', 9000);
  char* before_write_idx = buffer.writeBegin();
  buffer.append(buf, 9000);
  char* after_write_idx = buffer.writeBegin();
  assert(buffer.readableSize() == 9000);
  assert(0 == memcmp(buf, buffer.data(), 9000));
  assert(buffer.size() == 9000);

  // assert(9000 == after_write_idx - before_write_idx);
}

int main(int argc, char *argv[])
{
  TEST_apend();
  return 0;
}
