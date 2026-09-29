#ifndef AVION_MATH_VECTOR3_H
#define AVION_MATH_VECTOR3_H 1

#include <cmath>

namespace avion::math
{
  template <typename T>
  struct Vector3
  {
    T x{};
    T y{};
    T z{};

    Vector3() = default;
    Vector3(const T& value) : x(value), y(value), z(value) {}
    Vector3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {} 
    
    ~Vector3() = default;

    T Length() const noexcept
    {
      // TODO: type traits for check type and choose neccessary sqrt
      return std::sqrt(LengthSquared());
    }

    T LengthSquared() const noexcept
    {
      return x * x + y * y + z * z;
    }

    Vector3<T>& Normalize()
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
  };

  template <typename T>
  T Length(const Vector3<T>& v)
  {
    return std::sqrt(v.LengthSquared());
  }

  template <typename T>
  T LengthSquared(const Vector3<T>& v)
  {
    return v.LengthSquared();
  }

  template <typename T>
  void Normalize(Vector3<T>& v)
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
} // namespace avion::math

#endif 