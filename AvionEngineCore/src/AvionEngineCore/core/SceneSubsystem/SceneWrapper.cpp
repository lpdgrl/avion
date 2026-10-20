#include "AvionEngineCore/core/SceneSubsystem/SceneWrapper.hpp"
#include "AvionEngineCore/core/scene.hpp"

namespace avion::core::scene
{
  SceneWrapper::SceneWrapper(Scene& scene) : m_scene(scene) {}

  auto SceneWrapper::AddAttributeToEntity(EntityID id, Attribute attribute) -> bool
  {
    return m_scene.AddAttributeToEntity(id, attribute);
  }

  auto SceneWrapper::RemoveAttributeToEntity(EntityID id, Attribute attribute) -> bool
  {

  }

  auto SceneWrapper::GetEntity(EntityID id) -> Entity&
  {

  }

  auto SceneWrapper::GetEntities() -> void
  {

  }


} // namespace avion::core::scene
