#include "time_util.h"
#include <chrono>

/**
 * a utility class to work in milliseconds
 */
long long TimeUtil::now() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::high_resolution_clock::now().time_since_epoch())
      .count();
}
