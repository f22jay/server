/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/09/04 10:04:15
// Breif : simple loger
#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H
#include <stdio.h>

namespace common {
class Logger {
 public:
  Logger(FILE *file): file_(file) {}
  virtual ~Logger() {}
  void Log(const char* fmt, ...);
  void SetFile(FILE* file) {file_ = file;}

 private:
  FILE* file_;
};

extern Logger* g_logger;
#define LOG_INFO(format, args...)    \
  g_logger->Log("INFO    [%s:%d] [%s] "format, __FILE__, __LINE__, __FUNCTION__, ##args)
#define LOG_DEBUG(format, args...)   \
  g_logger->Log("DEBUG   [%s:%d] [%s] "format, __FILE__, __LINE__, __FUNCTION__, ##args)
#define LOG_WARNING(format, args...) \
  g_logger->Log("WARNING [%s:%d] [%s] "format, __FILE__, __LINE__, __FUNCTION__, ##args)
#define LOG_FATAL(format, args...)   \
  g_logger->Log("FATAL   [%s:%d] [%s] "format, __FILE__, __LINE__, __FUNCTION__, ##args)
} //namespace common
#endif
