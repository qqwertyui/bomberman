#pragma once

#include <cstdio>

namespace SimpleSnake {

#ifdef ENABLE_LOGS
#define LOG(...)                                                               \
  fprintf(stderr, __VA_ARGS__);                                                \
  fprintf(stderr, "\n");
#else
#define LOG(...)
#endif

} // namespace SimpleSnake