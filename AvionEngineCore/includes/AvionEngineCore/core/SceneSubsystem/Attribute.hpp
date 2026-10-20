#ifndef AVION_CORE_SCENE_SUBSYSTEM_ATTRIBUTE_H
#define AVION_CORE_SCENE_SUBSYSTEM_ATTRIBUTE_H 1

#include <cstdint>

namespace avion::core::scene
{

  enum class Attribute : std::uint8_t 
  {
    DirectionalLight = 0,
    PointLight,
    SpotLight,
    Camera, 
  };

} // namespace avion::core::scene

#endif