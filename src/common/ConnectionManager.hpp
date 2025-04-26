#pragma once

#include "ConnectionInfo.hpp"
#include "Networking.hpp"
#include "logging/Log.hpp"

#include <cstdint>
#include <optional>
#include <string>

namespace bm::common {
class ConnectionManager {
public:
  ConnectionManager() = default;
  ConnectionManager(const ConnectionInfo &info) : info(info) {}

  bool isConnected() const { return info.has_value(); }

  std::optional<ConnectionInfo> getConnectionInfo() { return info; }

  void connect(const std::string &ip, const uint16_t port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
      LOG_ERR("Socket creation failed: %s", strerror(errno));
      return;
    }
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (::connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
      LOG_ERR("Couldn't connect to %s:%u", ip.c_str(), port);
      return;
    }

    info =
        ConnectionInfo{fd, inet_ntoa(server.sin_addr), htons(server.sin_port)};
    LOG_DBG("Connected to %s:%u", info->ip.c_str(), info->port);
  }

  void disconnect() {
    if (not isConnected()) {
      return;
    }
    ::closeConnection(info->fd);
    info.reset();
  }

  template <typename Message> bool send(const Message &msg) {
    if (not isConnected()) {
      LOG_ERR("Not connected");
      return false;
    }
    std::string serializedMsg;
    msg.SerializeToString(&serializedMsg);
    unsigned int byteSizeLong = msg.ByteSizeLong();
    int status =
        ::send(info->fd, (const char *)&byteSizeLong, sizeof(byteSizeLong), 0);
    if (isConnectionError(status)) {
      return false;
    }
    status = ::send(info->fd, (const char *)serializedMsg.c_str(),
                    serializedMsg.size(), 0);
    if (isConnectionError(status)) {
      return false;
    }
    LOG_DBG("[send] msg:%s, size:%u, body:%s", msg.GetTypeName().c_str(),
            byteSizeLong, msg.DebugString().c_str());
    return true;
  }

  template <typename Message> std::optional<Message> receive() {
    if (not isConnected()) {
      LOG_ERR("Not connected");
      return std::nullopt;
    }
    unsigned int byteSizeLong;
    auto bytesReceived = recv(info->fd, (char *)&byteSizeLong,
                              sizeof(byteSizeLong), MSG_WAITALL);
    if (bytesReceived == 0) {
      return std::nullopt;
    }

    Message msg;
    std::string serializedMsg;
    serializedMsg.resize(byteSizeLong);
    recv(info->fd, (char *)serializedMsg.c_str(), byteSizeLong, MSG_WAITALL);
    msg.ParseFromString(serializedMsg);
    LOG_DBG("[recv] msg:%s, size:%u, body:%s", msg.GetTypeName().c_str(),
            byteSizeLong, msg.DebugString().c_str());
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
  std::optional<ConnectionInfo> info;
};
} // namespace bm::common
