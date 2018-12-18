// All Rights Reserved.
// Author : zhangfangjie
// Date 2018/07/09 17:40:29
// Breif :

#include <sys/time.h>
#include <stdint.h>
#include <time.h>

namespace commmon {
class Timer {
 public:
  Timer() {
    gettimeofday(&beg_tv_, NULL);
  }
  virtual ~Timer() {}

  static uint64_t get_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
  }

  static uint64_t get_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
  }

  /**
   * @brief get elpased time after timer is set
   * @param if reset is ture, reset beg_tv
   * @return us
   */
  uint64_t Elapse(bool reset = false) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    uint64_t cost_us =  (tv.tv_sec - beg_tv_.tv_sec) * 1000 * 1000 +
        (tv.tv_usec - beg_tv_.tv_usec);
    if (reset) {
      gettimeofday(&beg_tv_, NULL);
    }
    return cost_us;
  }

  void Reset() {
    gettimeofday(&beg_tv_, NULL);
  }

 private:
  struct timeval beg_tv_;
};

}  // common
