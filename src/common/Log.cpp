#include "Log.hpp"
#include "Utils.hpp"
#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace bm::common {
struct LogConfiguration {
  FILE *stream;
  const char *severityString;
};

static Level currentLevel{Level::INF};
static std::array<LogConfiguration, static_cast<unsigned int>(Level::SIZE)>
    profiles = {
        {{stderr, "ERR"}, {stderr, "WRN"}, {stdout, "INF"}, {stdout, "DBG"}}};

const LogConfiguration &getConfiguration(Level level) {
  return profiles[static_cast<unsigned int>(level)];
}

void setLogLevel(Level level) {
  assert(inRange(static_cast<unsigned int>(level), 0u,
                 static_cast<unsigned int>(Level::SIZE) - 1));
  currentLevel = level;
}

Level getLogLevel() { return currentLevel; }

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

void log(Level level, const char *path, int line, const char *format, ...) {
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

} // namespace bm::common
