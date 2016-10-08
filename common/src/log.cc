// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/09/04 11:29:06
// Breif :

#include "log.h"
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace common {

void Logger::Log(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char* buf= new char[500];
  char* limit = buf + 500;
  int tid = syscall(__NR_gettid);
  // try twice, if buffer is not enough ,then bigger
  for (int i = 0; i < 2; i++) {
    if (i == 1) {
      buf = new char[20000];
      limit = buf + 20000;
    }
    char* p = buf;
    struct timeval now_tv;
    gettimeofday(&now_tv, NULL);
    const time_t seconds = now_tv.tv_sec;
    struct tm t;
    localtime_r(&seconds, &t);
    p += snprintf(p, limit - p,
                  "%04d/%02d/%02d-%02d:%02d:%02d.%06d %x ",
                  t.tm_year + 1900,
                  t.tm_mon + 1,
                  t.tm_mday,
                  t.tm_hour,
                  t.tm_min,
                  t.tm_sec,
                  static_cast<int>(now_tv.tv_usec),
                  tid);
    va_list backup;
    va_copy(backup, ap);
    p += vsnprintf(p, limit - p, fmt, backup);
    *p = '\n';
    p++;

    //buf is full
    if (p >= limit) {
      if (i == 0) {
        continue;
      } else {
        p = limit - 1;
        //truncate log msg
      }
    }
    fwrite(buf, sizeof(char), p - buf, file_);
    break;
  }
}

FILE *g_log_file = stdout;
Logger* g_logger = new Logger(g_log_file);
} //namespace common
