// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/09/04 11:29:06
// Breif :

#include "log.h"
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace {
const char* g_level_chars[] = {
  "DEBUG   ",
  "INFO    ",
  "WARNING ",
  "FATAL   "
};
common::Level log_level_threhold = common::FATAL;
}  //

namespace common {
Logger* Logger::_logger = NULL;
Logger* Logger::GetLogger() {
  if (_logger == NULL) {
    _logger = new Logger(stdout);
  }
  return _logger;
}

void Logger::Log(Level level, const char* fmt, ...) {
  if (level < log_level_threhold) {
    return;
  }

  va_list ap;
  va_start(ap, fmt);
  char* buf= new char[500];
  char* limit = buf + 500;
  int tid = syscall(__NR_gettid);
  // try twice, if buffer is not enough ,then bigger
  for (int i = 0; i < 2; i++) {
    if (i == 1) {
      delete[] buf;
      buf = new char[20000];
      limit = buf + 20000;
    }
    char* p = buf;

    //print level
    int level_len = sizeof(g_level_chars[level]);
    strncpy(p, g_level_chars[level], level_len);
    p += level_len;

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

    //buf is full
    if (p >= limit) {
      if (i == 0) {
        continue;
      } else {
        p = limit - 1;
        //truncate log msg
      }
    }
    *p = '\n';
    p++;

    fwrite(buf, sizeof(char), p - buf, _file);
    break;
  }
  delete[] buf;
}

} //namespace common
