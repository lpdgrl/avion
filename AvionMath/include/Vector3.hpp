#ifndef AVION_MATH_VECTOR3_H
#define AVION_MATH_VECTOR3_H 1

#include <cmath>
#include <concepts>

namespace av::math::vec
{
  template <std::floating_point T>
  struct Vector3
  {
    T x{};
    T y{};
    T z{};

    constexpr Vector3() = default;
    explicit constexpr Vector3(const T& value) : x(value), y(value), z(value) {}
    explicit constexpr Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {} 
    
    explicit constexpr Vector3(const Vector3<T>& other) = default;
    explicit constexpr Vector3(Vector3<T>&& other) noexcept = default;

    constexpr Vector3<T>& operator=(const Vector3<T>& other) = default;
    constexpr Vector3<T>& operator=(Vector3<T>& other) noexcept = default;

    constexpr Vector3<T>& operator+=(const Vector3<T>& other) noexcept
    {
      if (&other == this)
      {
        return *this;
      }
        x += other.x;
        y += other.y;
        z += other.z;

      return *this;
    }

    constexpr ~Vector3() = default;

    [[nodiscard]] constexpr auto Length() const noexcept -> T
    {
      // TODO: type traits for check type and choose neccessary sqrt
      return std::sqrt(LengthSquared());
    }

    [[nodiscard]] constexpr auto LengthSquared() const noexcept -> T
    {
      return x * x + y * y + z * z;
    }

    constexpr auto Normalize() noexcept -> Vector3<T>&
    {
      T len = Length();
      if (len > 0)
      {
        T inv_len = 1 / len;
        x *= inv_len;
        y *= inv_len;
        z *= inv_len;
      }
      
      return *this;
    }

    [[nodiscard]] constexpr auto Dot(const Vector3<T>& v) const noexcept -> T
    {
      return x * v.x + y * v.y + z * v.z;
    }

    [[nodiscard]] constexpr auto Cross(const Vector3<T>& v) const noexcept -> Vector3<T>
    {
      return Vector3<T>(
        // Cx = Ay * Bz - Az * By
        y * v.z - z * v.y, 
        // Cy = Az * Bx - Ax * Bz
        z * v.x - x * v.z,
        // Cz = Ax * By - Ay * Bx
        x * v.y - y * v.x
      );
    }
  };

  template <typename T>
  [[nodiscard]] constexpr auto Length(const Vector3<T>& v) noexcept -> T
  {
    return std::sqrt(v.LengthSquared());
  }

  template <typename T>
  [[nodiscard]] constexpr auto LengthSquared(const Vector3<T>& v) noexcept -> T
  {
    return v.LengthSquared();
  }

  template <typename T>
  constexpr auto Normalize(Vector3<T>& v) noexcept -> void
  {
    T len = v.Length();
    if (len > 0)
    {
      T inv_len = 1 / len;
      v.x *= inv_len;
      v.y *= inv_len;
      v.z *= inv_len;
    }
  }

  template <typename T>
  [[nodiscard]] constexpr auto Dot(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept -> T
  {
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
  }

  template <typename T>
  [[nodiscard]] constexpr auto Cross(const Vector3<T>& a, const Vector3<T>& b) noexcept -> Vector3<T>
  {
    return Vector3<T>(
      // Cx = Ay * Bz - Az * By
      a.y * b.z - a.z * b.y, 
      // Cy = Az * Bx - Ax * Bz
      a.z * b.x - a.x * b.z,
      // Cz = Ax * By - Ay * Bx
      a.x * b.y - a.y * b.x
    );
  }

  template <typename T>
  [[nodiscard]] constexpr auto operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept -> Vector3<T>
  {
    return Vector3<T>(
      lhs.x + rhs.x,
      lhs.y + rhs.y,
      lhs.z + rhs.z
    );
  }

  template <typename T>
  [[nodiscard]] constexpr auto operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) noexcept -> Vector3<T>
  {
    return Vector3<T>(
      lhs.x - rhs.x,
      lhs.y - rhs.y,
      lhs.z - rhs.z
    );
  }

  template <typename T>
  [[nodiscard]] constexpr auto operator*(const Vector3<T>& lhs, const float scalar) noexcept -> Vector3<T>
  {
    return Vector3<T>(
      lhs.x * scalar,
      lhs.y * scalar,
      lhs.z * scalar
    );
  }


} // namespace av::math::vector

#endif 