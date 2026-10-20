#ifndef AVION_CORE_SUB_SYSTEM_SCENE_WRAPPER_H
#define AVION_CORE_SUB_SYSTEM_SCENE_WRAPPER_H 1

#include <AvionEngineCore/core/SceneSubsystem/Attribute.hpp>
#include <AvionEngineCore/core/SceneSubsystem/Entity.hpp>

// forward declaration
namespace avion::core
{
  class Scene;
} 

namespace avion::core::scene
{
  struct SceneWrapper
  {
    SceneWrapper(Scene& scene);
    
    auto AddAttributeToEntity(EntityID id, Attribute attribute) -> bool;
    auto RemoveAttributeToEntity(EntityID id, Attribute attribute) -> bool;

    auto GetEntity(EntityID id) -> Entity&;

    // Which type return???
    template <typename ReturnType>
    auto GetAttribute(EntityID id, Attribute attribute) -> ReturnType
    {

    }

    auto GetEntities() -> void;

    private:
      Scene& m_scene;
  };

} // namespace avion::core::scene

#endif