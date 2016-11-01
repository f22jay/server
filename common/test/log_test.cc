#include "log.h"
#include <string.h>
#include <gtest/gtest.h>

TEST(log, print) {
  char buf[10000] = "\0";
  memset(buf, 'c', 9999);
  for (int i = 0; i < 1000; i++) {
    common::LOG_INFO("hello world: %s", buf);
    common::LOG_INFO("hello world: %s", buf);
    common::LOG_DEBUG("hello world %d %d", i, ++i);
    common::LOG_WARNING("hello world");
    common::LOG_FATAL("hello world");
  }
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
