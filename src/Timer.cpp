#include "Timer.hpp"

namespace SimpleSnake {

Timer::Timer(const double intervalMs) : m_intervalMs(intervalMs) {
  m_start = std::chrono::high_resolution_clock::now();
}

void Timer::reset() { m_start = std::chrono::high_resolution_clock::now(); }

bool Timer::nextTick() const {
  auto now = std::chrono::high_resolution_clock::now();
  if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start)
          .count() > m_intervalMs) {
    return true;
  }
  return false;
}

} // namespace SimpleSnake