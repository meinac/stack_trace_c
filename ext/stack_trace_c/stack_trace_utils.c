#include <time.h>

static struct timespec time_v = {};

long monotonic_time() {
  clock_gettime(CLOCK_MONOTONIC, &time_v);

  return (time_v.tv_sec * 1000000 + time_v.tv_nsec / 1000);
}
