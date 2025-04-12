#pragma once

#include "common/ConnectionManager.hpp"
#include "common/itf/core.pb.h"
#include <array>
#include <string>
#include <thread>

namespace bm::scene::game {
class MessageHandler {
public:
  MessageHandler(const std::string &ip, unsigned int port);
  ~MessageHandler();

  bool isConnected() const;
  void handle();
  bool send(common::itf::C2SMessage &req);

private:
  void handleMessage(const common::itf::S2CMessage &msg);
  void handleQuery(const common::itf::QueryResp &msg);
  void handleUpdate(const common::itf::UpdateResp &msg);
  void handleInd(const common::itf::UpdateInd &msg);

  static void threadMainLoop(MessageHandler *self);

  static constexpr unsigned int queueSize{100};
  struct Buffer {
    std::array<common::itf::S2CMessage, queueSize> elements;
    int size{};
  };

  Buffer &getActiveBuffer();
  Buffer &getPassiveBuffer();
  void swapBuffers();
  int getNextIndex(int index) const;
  bool push(Buffer &buffer, common::itf::S2CMessage &element);

  static constexpr unsigned int numberOfBuffers{2};
  std::array<Buffer, numberOfBuffers> buffers;
  unsigned int activeBufferIndex, passiveBufferIndex;

  common::ConnectionManager connMgr;
  std::thread serverMessageHandler;
};

} // namespace bm::scene::game
