#pragma once

namespace bm::common {
class NonCopyable {
protected:
  NonCopyable() {}

private:
  NonCopyable(const NonCopyable &);

  NonCopyable &operator=(const NonCopyable &);
};
} // namespace bm::common
