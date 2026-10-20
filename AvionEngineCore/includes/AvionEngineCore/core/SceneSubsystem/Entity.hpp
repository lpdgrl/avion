#ifndef AVION_CORE_SCENE_SUBSYSTEM_ENTITY_H
#define AVION_CORE_SCENE_SUBSYSTEM_ENTITY_H 1

#include <string>
#include <optional>

#include "AvionEngineCore/renderer/transform.hpp"
#include "AvionEngineCore/core/material.hpp"

namespace avion::core::scene
{
  using EntityID = std::uint64_t;
  using Material = core::material::Material;

  struct Entity
  {
    EntityID                entity_id{};
    gfx::Transform          transform;
    std::optional<Material> material_override;
    std::string             name;
  };
} // namespace avion::core::scene

#endif