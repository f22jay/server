//-*-c++-*-
// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : zhangfangjie (zhangfangjie@baidu.com)
// Date 2017/05/03 14:14:57
// Breif :

#include <thread>

void thread_run() {
  while (true);
}
int main(int argc, char *argv[])
{
  std::thread first(thread_run);
  std::thread two(thread_run);
  std::thread three(thread_run);
  std::thread four(thread_run);
  std::thread five(thread_run);
  std::thread six(thread_run);

  first.join();
  two.join();
  three.join();
  return 0;
}
