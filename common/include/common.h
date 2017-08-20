//-*-c++-*-
// All Rights Reserved.
// Author : zhangfangjie
// Date 2017/08/20 21:27:52
// Breif :
#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H
#include "log.h"
namespace common {
#define CHECK(EXPRESSION, ret, fmt, ...) do {\
    if (!(EXPRESSION)) { \
        LOG_WARNING(fmt, ##__VA_ARGS__); \
        return ret; \
    } \
} while (0)

#define CHECK_EQ(A, B, ret, fmt, ...)  CHECK((A) == (B), ret, fmt, ##__VA_ARGS__)
#define CHECK_NE(A, B, ret, fmt, ...)  CHECK((A) != (B), ret, fmt, ##__VA_ARGS__)
#define CHECK_LT(A, B, ret, fmt, ...)  CHECK((A) < (B), ret, fmt, ##__VA_ARGS__)
#define CHECK_LE(A, B, ret, fmt, ...)  CHECK((A) <= (B), ret, fmt, ##__VA_ARGS__)
#define CHECK_GT(A, B, ret, fmt, ...)  CHECK((A) > (B), ret, fmt, ##__VA_ARGS__)
#define CHECK_GE(A, B, ret, fmt, ...)  CHECK((A) >= (B), ret, fmt, ##__VA_ARGS__)

#define CHECK_RET(A, ret, fmt, ...) CHECK_EQ(0, A, ret, fmt, ##__VA_ARGS__)
#define CHECK_POINTER(A, ret, fmt, ...) CHECK_NE(NULL, A, ret, fmt, ##__VA_ARGS__)
#define CHECK_TRUE(A, ret, fmt, ...) CHECK_EQ(true, A, ret, fmt, ##__VA_ARGS__)
}  // common

#endif
