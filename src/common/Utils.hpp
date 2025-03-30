#pragma once

namespace bm::common {
template <typename T> bool inRange(const T &value, const T &min, const T &max) {
  return (value >= min and value <= max);
}

} // namespace bm::common
