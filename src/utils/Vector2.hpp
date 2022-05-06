#pragma once

namespace SimpleSnake {

template <typename T> class Vector2 {
public:
  constexpr Vector2() = default;
  constexpr explicit Vector2(T x, T y) : x(x), y(y) {}
  T x, y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<double> Vector2d;
typedef Vector2<float> Vector2f;

} // namespace SimpleSnake