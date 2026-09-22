#ifndef AVION_CORE_SERIALIZATION_ENTITY_SERIALIZE_H
#define AVION_CORE_SERIALIZATION_ENTITY_SERIALIZE_H 1

#include <cstdint>

namespace avion::core::serialization::detail
{
  struct Transform 
  {
    std::array<float, 3> position;
    std::array<float, 3> rotation;
    std::array<float, 3> size;

    float rotate_value{};
    int axis_rotate = -1;
  };

  struct Color 
  {
    float red{};
    float green{};
    float blue{};
  };

  struct Material
  {
    Color color;
    float shininess{};
    std::uint8_t material_type{};
  };

  struct EntitySerialize
  {
    Transform transform;
    Material material;

    std::string filename_model;
    std::uint8_t type{};
  };

} // namespace avion::core::serialization::detail

#endif