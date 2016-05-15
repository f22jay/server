// Copyright 2016 Baidu Inc. All Rights Reserved.
// Author : moon (moon@baidu.com)
// 
// Breif : simple log to file

#ifndef LOGGING_H
#define LOGGING_H

#incldue <sys/time.h>
#include <time.h>
#include <stdio.h>

namespace base {
class Log {
private:
    FILE * _fd;
    uint64_t (* _get_tid)();
    static const uint64_t kBufferSize = 1000;
    char _buffer[KBufferSize];

public:
    Log(const char *file, uint64_t (*get_id)()) : _fd(fd), _get_tid(get_tid) {}
    virtual ~Log() {}
    
    void log(const char *format, va_list ap) {
        const uint64_t thread_id = (*_get_id)();
        char *p = _buffer;
    }
};

static const LOG *g_logger = new LOG();

#define LOG_INFO()  //TODO
}



















