#include "Log.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace bomberman::common {
void log(FILE *stream, const char *severity, const char *path, int line,
         const char *format, ...) {
  timespec ts;
  timespec_get(&ts, TIME_UTC);
  char time_buf[100];
  size_t rc = strftime(time_buf, sizeof(time_buf), "%FT%T", gmtime(&ts.tv_sec));
  snprintf(time_buf + rc, sizeof time_buf - rc, ".%06ldZ", ts.tv_nsec / 1000);

  const char *filename = basename(path);
  fprintf(stream, "<%s> %s/%s:%d ", time_buf, severity, filename, line);

  va_list args;
  va_start(args, format);
  vfprintf(stream, format, args);
  va_end(args);

  fprintf(stream, "\n");
}

} // namespace bomberman::common
