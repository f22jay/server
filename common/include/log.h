/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/09/04 10:04:15
// Breif : simple loger
#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H
#include <stdio.h>

namespace common {
enum Level {
  DEBUG = 0,
  INFO,
  WARNING,
  FATAL
};

class Logger {
 public:
  virtual ~Logger() {}
  void Log(Level level, const char* fmt, ...);
  void SetFile(FILE* file) {file_ = file;}
  static Logger* GetLogger();

 private:
  Logger(FILE* file): file_(file) {}

 private:
  FILE* file_;
  static Logger* logger_;
};

#define LOG(level, format, ...) \
  Logger::GetLogger()->Log(level, "[%s:%d] [%s] " format, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG(common::DEBUG, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG(common::INFO, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) LOG(common::WARNING, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) LOG(common::FATAL, format, ##__VA_ARGS__)

// #define LOG_DEBUG(format, args...)   \
//   Logger::GetLogger()->Log("DEBUG   [%s:%d] [%s] " format, __FILE__, __LINE__, __FUNCTION__, ##args)
// #define LOG_WARNING(format, args...) \
//   Logger::GetLogger()->Log("WARNING [%s:%d] [%s] " format, __FILE__, __LINE__, __FUNCTION__, ##args)
// #define LOG_FATAL(format, args...)   \
//   Logger::GetLogger()->Log("FATAL   [%s:%d] [%s] " format, __FILE__, __LINE__, __FUNCTION__, ##args)
} //namespace common
#endif
