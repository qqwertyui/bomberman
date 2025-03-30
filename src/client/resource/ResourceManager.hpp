#pragma once

#include "common/Log.hpp"
#include <map>
#include <stdexcept>

namespace bm::resource {
template <typename ResourceType, typename ResourceIdType>
class ResourceManager {
public:
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager(ResourceManager &&) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;
  ResourceManager &operator=(ResourceManager &&) = delete;

  static ResourceManager<ResourceType, ResourceIdType> &get() {
    static ResourceManager<ResourceType, ResourceIdType> instance;
    return instance;
  }

  void load(const ResourceIdType id, const std::string &path) {
    auto &rsrc{resourceMap[id]};
    try {
      rsrc = ResourceType{path};
      LOG_INF("Loaded resource: %s", path.c_str());
    } catch (const std::exception &) {
      LOG_ERR("Failed to load %s", path.c_str());
    }
  }

  ResourceType &at(const ResourceIdType id) {
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

}; // namespace bm::resource
