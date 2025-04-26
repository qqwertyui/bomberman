#pragma once

#include "common/GlobalConfigBase.hpp"
#include "common/logging/Level.hpp"

namespace bm {
struct Params {
  PARAMETER(uint16_t, maxFps, 60);
  PARAMETER(std::string, assetsDir, "assets/");
  PARAMETER(std::string, serverIp, "127.0.0.1");
  PARAMETER(uint16_t, serverPort, 1111);
  PARAMETER(uint16_t, windowWidth, 640);
  PARAMETER(uint16_t, windowHeight, 480);
  PARAMETER(unsigned int, logLevel,
            static_cast<unsigned int>(common::logging::Level::INF));
};

using GlobalConfig = common::GlobalConfigBase<Params>;

} // namespace bm
