#pragma once

#include "common/GlobalConfigBase.hpp"

namespace bm {
struct Params {
  PARAMETER(std::string, bindIp, "0.0.0.0");
  PARAMETER(uint16_t, port, 1111);
  PARAMETER(uint16_t, backlogSize, 128);
  PARAMETER(uint16_t, maxLobbySize, 2);
  PARAMETER(int, maxNumberOfLobbies, 10);
  PARAMETER(int, maxNumberOfPlayers, 100);
  PARAMETER(int, version, 1);
  PARAMETER(int, playerLimitEvaluationTimerMs, 1000);
};

using GlobalConfig = common::GlobalConfigBase<Params>;

} // namespace bm
