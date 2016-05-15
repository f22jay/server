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

/* vim: set ts=4 sw=4 sts=4 tw=100 */
#include <iostream>
#include <pthread.h>
#include <syscall.h>
#include "../include/thread_pool.h"

void test() {
	std::cout<<"thread pid is :"<<syscall(__NR_gettid)<<std::endl;
}

void test_thread_pool() {
    common::ThreadPool threadPool(100);
    threadPool.Start();
    for(int i = 0; i<1000000; i++) {
        threadPool.AddTask(test);
    }
    while (true) ;
}

int main() {
     test_thread_pool();
     return 0;
}
