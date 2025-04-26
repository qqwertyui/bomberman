#pragma once

#include "LogLevel.hpp"

namespace bm::common {
void log(Level level, const char *file, int line, const char *format, ...);
void setLogLevel(Level level);
Level getLogLevel();
} // namespace bm::common

#define LOG_INF(...)                                                           \
  bm::common::log(bm::common::Level::INF, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WRN(...)                                                           \
  bm::common::log(bm::common::Level::WRN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...)                                                           \
  bm::common::log(bm::common::Level::ERR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DBG(...)                                                           \
  bm::common::log(bm::common::Level::DBG, __FILE__, __LINE__, __VA_ARGS__)
