#pragma once

#include <utility>

namespace bm::common {
template <typename Enum> constexpr decltype(auto) toUnderlying(const Enum &e) {
  return static_cast<std::underlying_type_t<Enum>>(e);
}
} // namespace bm::common
