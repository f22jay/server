/***************************************************************************
 *
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * $Id$
 *
 **************************************************************************/

/**
 * @file test/test.cpp
 * @author zhangfangjie(zhangfangjie@baidu.com)
 * @date 2015/11/19 10:56:21
 * @version $Revision$
 * @brief
 *
 **/

#include "thread_pool.h"
#include "log.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

void test() {
  char buf[40000] = "\0";
  memset(buf, 'c', 39999);
  for (int i = 0; i < 10; i++) {
    common::LOG_INFO("hello world %s", buf);
    common::LOG_DEBUG("hello world %d %d", i, ++i);
    common::LOG_WARNING("hello world");
    common::LOG_FATAL("hello world");
  }
}

void test_thread_pool() {
  common::ThreadPool threadPool(10);
  threadPool.Start();
  for(int i = 0; i<1000; i++) {
    threadPool.AddTask(test);
  }
  usleep(20000000);
}

int main() {
  test_thread_pool();
  return 0;
}
