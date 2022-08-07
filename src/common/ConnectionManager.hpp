#pragma once

#include "common/Log.hpp"
#include "common/Networking.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace bomberman::common {
class ConnectionManager {
public:
  ConnectionManager() = default;
  ConnectionManager(int fileDescriptor) : fileDescriptor(fileDescriptor) {}

  void connect(const std::string &ip, const uint16_t port) {
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
      LOG_ERR("Socket creation failed: %s", strerror(errno));
      return;
    }
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (::connect(socket_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
      LOG_ERR("Couldn't connect to %s:%u", ip.c_str(), port);
      return;
    }
    fileDescriptor = socket_fd;
    LOG_DBG("Connected");
  }

  void disconnect() {
    if (not fileDescriptor.has_value()) {
      LOG_WRN("Connection already closed");
      return;
    }
    ::closeConnection(fileDescriptor.value());
    fileDescriptor.reset();
    LOG_DBG("Disconnected");
  }

  template <typename Message> void send(const Message &msg) {
    if (not fileDescriptor.has_value()) {
      LOG_ERR("Not connected");
      return;
    }
    std::string serializedMsg;
    msg.SerializeToString(&serializedMsg);
    unsigned int byteSizeLong = msg.ByteSizeLong();
    int status = ::send(fileDescriptor.value(), (const char *)&byteSizeLong,
                        sizeof(byteSizeLong), 0);
    if (isConnectionError(status)) {
      return;
    }
    status = ::send(fileDescriptor.value(), (const char *)serializedMsg.c_str(),
                    serializedMsg.size(), 0);
    if (isConnectionError(status)) {
      return;
    }
    LOG_DBG("Sent message %s [%u bytes]", msg.GetTypeName().c_str(),
            byteSizeLong);
  }

  template <typename Message> std::optional<Message> receive() {
    if (not fileDescriptor.has_value()) {
      LOG_ERR("Not connected");
      return std::nullopt;
    }
    Message msg;
    unsigned int byteSizeLong;
    std::string serializedMsg;
    ::recv(fileDescriptor.value(), (char *)&byteSizeLong, sizeof(byteSizeLong),
           MSG_WAITALL);
    serializedMsg.resize(byteSizeLong);
    ::recv(fileDescriptor.value(), (char *)serializedMsg.c_str(), byteSizeLong,
           MSG_WAITALL);
    msg.ParseFromString(serializedMsg);
    LOG_DBG("Received message %s [%u bytes]", msg.GetTypeName().c_str(),
            byteSizeLong);
    return msg;
  }

  bool isConnectionError(int statusCode) {
    if (statusCode == -1) {
      LOG_ERR("Connection error: %s", strerror(statusCode));
      disconnect();
      return true;
    }
    return false;
  }

private:
  std::optional<int> fileDescriptor;
};
} // namespace bomberman::common