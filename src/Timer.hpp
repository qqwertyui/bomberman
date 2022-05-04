#pragma once
#include <chrono>

namespace SimpleSnake {

class Timer {
public:
  Timer(const double intervalMs);
  void reset();
  bool nextTick() const;

private:
  const double m_intervalMs;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

} // namespace SimpleSnake