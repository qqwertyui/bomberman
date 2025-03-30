#pragma once

namespace bm {
class Application {
public:
  int run(int argc, char **argv);

private:
  bool initialize(int argc, char **argv);
};

} // namespace bm
