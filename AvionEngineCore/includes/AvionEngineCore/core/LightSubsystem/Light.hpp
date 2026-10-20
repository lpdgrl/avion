#ifndef AVION_CORE_LIGHT_SUBSYSTEM_LIGHT_H
#define AVION_CORE_LIGHT_SUBSYSTEM_LIGHT_H 1

#include "../../../../../AvionMath/include/Vector3.hpp"

namespace avion::core::light
{
  using Vec3f = av::math::vec::Vector3<float>;

  struct DirectionalLight
  {
    Vec3f direction;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
  };

  struct PointLight
  {
    Vec3f position;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;

    float constant;
    float linear;
    float quadratic;
  };

  struct SpotLight
  {
    Vec3f position;
    Vec3f direction;
    Vec3f ambient;
    Vec3f diffuse;
    Vec3f specular;
    
    float constant;
    float linear;
    float quadratic;
    float cutoff;
    float outer_cutoff;
  };

} // namespace avion::core

#endif