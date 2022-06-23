#pragma once

#include <cstdio>

namespace bomberman::common {

#ifdef ENABLE_LOGS
#ifndef NDEBUG
#define LOG_DBG(...)                                                           \
  fprintf(stdout, "[DEBUG] ");                                                 \
  fprintf(stdout, __VA_ARGS__);                                                \
  fprintf(stdout, "\n");
#else
#define LOG_DBG(...)
#endif

#define LOG_INF(...)                                                           \
  fprintf(stdout, "[*] ");                                                     \
  fprintf(stdout, __VA_ARGS__);                                                \
  fprintf(stdout, "\n");

#define LOG_WRN(...)                                                           \
  fprintf(stderr, "[!] ");                                                     \
  fprintf(stderr, __VA_ARGS__);                                                \
  fprintf(stderr, "\n");

#define LOG_ERR(...)                                                           \
  fprintf(stderr, "[-] ");                                                     \
  fprintf(stderr, __VA_ARGS__);                                                \
  fprintf(stderr, "\n");
#else
#define LOG_DBG(...)
#define LOG_INF(...)
#define LOG_WRN(...)
#define LOG_ERR(...)
#endif

} // namespace bomberman::common