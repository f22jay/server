#include "log.h"
#include "common.h"
#include <string.h>
#include <gtest/gtest.h>
using namespace common;

int test() {
    return 0;
}

TEST(common, check) {
    // CHECK_EQ(0, test(), 1, "hello");
    ASSERT_EQ(0, test());
}
TEST(log, print) {
  char buf[10000] = "\0";
  memset(buf, 'c', 9999);
  for (int i = 0; i < 1; i++) {
    common::LOG_INFO("hello world: %s", buf);
    common::LOG_INFO("hello world: %s", buf);
    common::LOG_DEBUG("hello world %d %d", i, ++i);
    common::LOG_WARNING("hello world");
    common::LOG_FATAL("hello world");
  }

  EXPECT_EQ(0, test());
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
