#pragma once

#include <cstdio>

namespace bomberman::common {
void log(FILE *stream, const char *severity, const char *file, int line,
         const char *format, ...);
}

#define LOG_INF(...)                                                           \
  bomberman::common::log(stdout, "INF", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WRN(...)                                                           \
  bomberman::common::log(stderr, "WRN", __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...)                                                           \
  bomberman::common::log(stderr, "ERR", __FILE__, __LINE__, __VA_ARGS__)

#ifndef NDEBUG
#define LOG_DBG(...)                                                           \
  bomberman::common::log(stdout, "DBG", __FILE__, __LINE__, __VA_ARGS__)
#else
#define LOG_DBG(...)
#endif

// namespace bomberman::common
