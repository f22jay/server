#include "log.h"
#include <gtest/gtest.h>

TEST(log, print) {
  for (int i = 0; i < 100; i++) {
    common::LOG_INFO("hello world");
    common::LOG_DEBUG("hello world %d %d", i, ++i);
    common::LOG_WARNING("hello world");
    common::LOG_FATAL("hello world");
  }
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
