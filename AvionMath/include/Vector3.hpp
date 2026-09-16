#ifndef AVION_MATH_VECTOR3_H
#define AVION_MATH_VECTOR3_H 1

  namespace avion::math
  {
    template <typename T>
    struct Vector3
    {
      Vector3() = default;
      Vector3(const T& value) : x(value), y(value), z(value) {}
      Vector3(const T& xx, const T& yy, const T& zz) : x(xx), y(yy), z(zz) {} 
      
      T x{};
      T y{};
      T z{};
    };
  } // namespace avion::math

#endif 