#ifndef AVION_CORE_SERIALIZATION_ENTITY_SERIALIZE_H
#define AVION_CORE_SERIALIZATION_ENTITY_SERIALIZE_H 1

#include <cstdint>
#include <optional>
#include <vector>

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

  struct DirLightSerialize
  {
    Transform transform;

    std::string filename;

    std::array<float, 3> direction;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;
    
    std::uint8_t type{};
  };

  struct PointLightSerialize
  {
    Transform transform;

    std::string filename;

    std::array<float, 3> position;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    float constant{};
    float linear{};
    float quadratic{};

    std::uint8_t type{};
  };

  struct SpotLightSerialize
  {
    Transform transform;

    std::string filename;


    std::array<float, 3> position;
    std::array<float, 3> direction;
    std::array<float, 3> ambient;
    std::array<float, 3> diffuse;
    std::array<float, 3> specular;

    float constant{};
    float linear{};
    float quadratic{};
    float cutoff{};
    float outer_cutoff;

    std::uint8_t type{};
  };

  struct SceneSerialize
  {
    std::vector<EntitySerialize>                    entities;
    std::optional<std::vector<DirLightSerialize>>   dir_light_entities;
    std::optional<std::vector<PointLightSerialize>> point_light_entities;
    std::optional<std::vector<SpotLightSerialize>>  spot_light_entities;
  };

} // namespace avion::core::serialization::detail

#endif