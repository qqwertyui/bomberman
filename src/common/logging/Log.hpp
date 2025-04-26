#pragma once

#include "Level.hpp"

namespace bm::common::logging {
void write(Level level, const char *file, int line, const char *format, ...);
void setLevel(Level level);
Level getLevel();
} // namespace bm::common::logging

#define LOG_INF(...)                                                           \
  bm::common::logging::write(bm::common::logging::Level::INF, __FILE__,        \
                             __LINE__, __VA_ARGS__)
#define LOG_WRN(...)                                                           \
  bm::common::logging::write(bm::common::logging::Level::WRN, __FILE__,        \
                             __LINE__, __VA_ARGS__)
#define LOG_ERR(...)                                                           \
  bm::common::logging::write(bm::common::logging::Level::ERR, __FILE__,        \
                             __LINE__, __VA_ARGS__)
#define LOG_DBG(...)                                                           \
  bm::common::logging::write(bm::common::logging::Level::DBG, __FILE__,        \
                             __LINE__, __VA_ARGS__)
