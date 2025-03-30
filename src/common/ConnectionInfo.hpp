#pragma once

#include <string>

namespace bm::common {
struct ConnectionInfo {
  int fd;
  std::string ip;
  unsigned int port;
};

} // namespace bm::common
