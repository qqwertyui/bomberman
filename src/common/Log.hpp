#pragma once

#include <cstdio>

namespace bm::common {
void log(FILE *stream, const char *severity, const char *file, int line,
         const char *format, ...);
}

#define LOG_INF(...)                                                           \
  bm::common::log(stdout, "INF", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WRN(...)                                                           \
  bm::common::log(stderr, "WRN", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...)                                                           \
  bm::common::log(stderr, "ERR", __FILE__, __LINE__, __VA_ARGS__)

#ifndef NDEBUG
#define LOG_DBG(...)                                                           \
  bm::common::log(stdout, "DBG", __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_DBG(...)
#endif

// namespace bm::common
