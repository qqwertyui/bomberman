#pragma once

#include "common/Log.hpp"
#include <iostream>
#include <map>

namespace bomberman::rsrcManagement {
template <typename ResourceType, typename ResourceIdType>
class ResourceManager {
public:
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager(ResourceManager &&) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ResourceManager &operator=(ResourceManager &&) = delete;

  static ResourceManager<ResourceType, ResourceIdType> &instance() {
    static ResourceManager<ResourceType, ResourceIdType> instance;
    return instance;
  }

  void load(const ResourceIdType id, const std::string &path) {
    auto &rsrc{resourceMap[id]};
    if (not rsrc.loadFromFile(path)) {
      LOG_ERR("Failed to load %s", path.c_str());
      return;
    }
    LOG_INF("Loaded resource: %s", path.c_str());
  }

  ResourceType &get(const ResourceIdType id) {
    if (resourceMap.count(id) == 0) {
      LOG_WRN("Couldn't retreive resource");
      return invalidResource;
    }
    return resourceMap.at(id);
  }

private:
  ResourceManager() = default;
  std::map<ResourceIdType, ResourceType> resourceMap{};
  ResourceType invalidResource{};
};
}; // namespace bomberman::rsrcManagement