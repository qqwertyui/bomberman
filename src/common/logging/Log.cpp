#include "Log.hpp"
#include "common/ToUnderlying.hpp"
#include "common/Utils.hpp"
#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace bm::common::logging {
struct Configuration {
  FILE *stream;
  const char *severityString;
};

static Level currentLevel{Level::INF};
static std::array<Configuration, toUnderlying(Level::SIZE)> profiles = {
    {{stderr, "ERR"}, {stderr, "WRN"}, {stdout, "INF"}, {stdout, "DBG"}}};

const Configuration &getConfiguration(Level level) {
  return profiles[toUnderlying(level)];
}

void setLevel(Level level) {
  assert(inRange(toUnderlying(level), 0u, toUnderlying(Level::SIZE) - 1));
  currentLevel = level;
}

Level getLevel() { return currentLevel; }

bool checkLevel(Level level) { return (level <= currentLevel); }

std::array<char, 100> getTime() {
  std::array<char, 100> buffer;
  timespec ts;
  timespec_get(&ts, TIME_UTC);

  size_t rc =
      strftime(buffer.data(), buffer.size(), "%FT%T", gmtime(&ts.tv_sec));
  snprintf(buffer.data() + rc, sizeof buffer.data() - rc, ".%06ldZ",
           ts.tv_nsec / 1000);
  return buffer;
}

void write(Level level, const char *path, int line, const char *format, ...) {
  if (not checkLevel(level)) {
    return;
  }
  const auto &cfg = getConfiguration(level);
  const auto &time_buf = getTime();
  const char *filename = basename(path);

  va_list args;
  va_start(args, format);
  fprintf(cfg.stream, "<%s> %s/%s:%d ", time_buf.data(), cfg.severityString,
          filename, line);
  vfprintf(cfg.stream, format, args);
  fprintf(cfg.stream, "\n");
  va_end(args);
}

} // namespace bm::common::logging
