#include "MessageHandler.hpp"

#include "common/Log.hpp"
#include <algorithm>
#include <span>

namespace bm::scene::game {
MessageHandler::MessageHandler(const std::string &ip, unsigned int port) {
  activeBufferIndex = 0;
  passiveBufferIndex = 1;

  connMgr.connect(ip, port);
  if (not isConnected()) {
    return;
  }
  serverMessageHandler = std::thread{threadMainLoop, this};
}

MessageHandler::~MessageHandler() {
  connMgr.disconnect();
  serverMessageHandler.join();
}

bool MessageHandler::isConnected() const { return connMgr.isConnected(); }

MessageHandler::Buffer &MessageHandler::getActiveBuffer() {
  return buffers[activeBufferIndex];
}

MessageHandler::Buffer &MessageHandler::getPassiveBuffer() {
  return buffers[passiveBufferIndex];
}

void MessageHandler::swapBuffers() {
  std::swap(activeBufferIndex, passiveBufferIndex);
}

bool MessageHandler::push(MessageHandler::Buffer &buffer,
                          common::itf::S2CMessage &element) {
  auto &index{buffer.size};
  bool isFull{index == static_cast<int>(buffer.elements.size())};
  if (isFull) {
    return false;
  }
  buffer.elements[index] = element;
  index++;
  return true;
}

void MessageHandler::handle() {
  auto &buffer{getPassiveBuffer()};
  std::span<common::itf::S2CMessage> messages{
      buffer.elements.data(), buffer.elements.data() + buffer.size};

  for (auto &msg : messages) {
    const bool ack{msg.has_update() and msg.update().has_game() and
                   msg.update().game().has_ispositionok() and
                   msg.update().game().ispositionok()};
    LOG_DBG(ack ? "ack" : "nack");
  }

  buffer.size = 0;
}

bool MessageHandler::send(common::itf::C2SMessage &req) {
  return connMgr.send(req);
}

void MessageHandler::threadMainLoop(MessageHandler *self) {
  while (auto resp = self->connMgr.receive<common::itf::S2CMessage>()) {
    auto &activeBuffer = self->getActiveBuffer();
    if (not self->push(activeBuffer, resp.value())) {
      LOG_ERR("Not enough space in rx buffer, some messages will be lost!");
    }

    auto &passiveBuffer = self->getPassiveBuffer();
    if (passiveBuffer.size <= 0) {
      self->swapBuffers();
    }
  }
}

} // namespace bm::scene::game
