#pragma once

#include "common/GlobalConfigBase.hpp"

namespace bomberman {
class GlobalConfig : public GlobalConfigBase {
public:
  using GlobalConfigBase::GlobalConfigBase;

  inline static GlobalConfig &get() {
    static GlobalConfig globalConfig;
    return globalConfig;
  }

  PARAMETER(uint16_t, maxLobbySize, 2);
  PARAMETER(int, numberOfLobbies, 10);
  PARAMETER(int, version, 1);

private:
  GlobalConfig() = default;
};

} // namespace bomberman