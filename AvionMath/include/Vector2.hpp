#ifndef AVION_MATH_VECTOR2_H
#define AVION_MATH_VECTOR2_H 1

#include <cmath>
#include <concepts>

namespace av::math::vec
{
  template <std::floating_point T>
  struct Vector2
  {
    T x{};
    T y{};

    constexpr Vector2() = default;
    explicit constexpr Vector2(const T& value) : x(value), y(value) {}
    explicit constexpr Vector2(T xx, T yy) : x(xx), y(yy) {} 
    
    constexpr Vector2(const Vector2<T>& other) = default;
    constexpr Vector2(Vector2<T>&& other) noexcept = default;

    constexpr Vector2<T>& operator=(const Vector2<T>& other) = default;
    constexpr Vector2<T>& operator=(Vector2<T>& other) noexcept = default;

    constexpr Vector2<T>& operator+=(const Vector2<T>& other) noexcept
    {
      if (&other == this)
      {
        return *this;
      }
      x += other.x;
      y += other.y;
      return *this;
    }

    constexpr ~Vector2() = default;

    [[nodiscard]] constexpr auto Length() const noexcept -> T
    {
      // TODO: type traits for check type and choose neccessary sqrt
      return std::sqrt(LengthSquared());
    }

    [[nodiscard]] constexpr auto LengthSquared() const noexcept -> T
    {
      return x * x + y * y ;
    }

    constexpr auto Normalize() noexcept -> Vector2<T>&
    {
      T len = Length();
      if (len > 0)
      {
        T inv_len = 1 / len;
        x *= inv_len;
        y *= inv_len;
      }
      
      return *this;
    }

    [[nodiscard]] constexpr auto Dot(const Vector2<T>& v) const noexcept -> T
    {
      return x * v.x + y * v.y;
    }
  };

  template <typename T>
  [[nodiscard]] constexpr auto Length(const Vector2<T>& v) noexcept -> T
  {
    return std::sqrt(v.LengthSquared());
  }

  template <typename T>
  [[nodiscard]] constexpr auto LengthSquared(const Vector2<T>& v) noexcept -> T
  {
    return v.LengthSquared();
  }

  template <typename T>
  constexpr auto Normalize(Vector2<T>& v) noexcept -> void
  {
    T len = v.Length();
    if (len > 0)
    {
      T inv_len = 1 / len;
      v.x *= inv_len;
      v.y *= inv_len;
    }
  }

  template <typename T>
  [[nodiscard]] constexpr auto Dot(const Vector2<T>& lhs, const Vector2<T>& rhs) noexcept -> T
  {
    return lhs.x * rhs.x + lhs.y * rhs.y;
  }

  template <typename T>
  [[nodiscard]] constexpr auto operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) noexcept -> Vector2<T>
  {
    return Vector2<T>(
      lhs.x + rhs.x,
      lhs.y + rhs.y
    );
  }

  template <typename T>
  [[nodiscard]] constexpr auto operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) noexcept -> Vector2<T>
  {
    return Vector2<T>(
      lhs.x - rhs.x,
      lhs.y - rhs.y
    );
  }

  template <typename T>
  [[nodiscard]] constexpr auto operator*(const Vector2<T>& lhs, const float scalar) noexcept -> Vector2<T>
  {
    return Vector2<T>(
      lhs.x * scalar,
      lhs.y * scalar
    );
  }


} // namespace av::math::vector

#endif 