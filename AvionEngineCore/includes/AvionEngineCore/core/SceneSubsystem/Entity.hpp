#ifndef AVION_CORE_SCENE_SUBSYSTEM_ENTITY_H
#define AVION_CORE_SCENE_SUBSYSTEM_ENTITY_H 1

  namespace avion::core::scene
  {
    struct Entity
    {
      EntityId entity_id{};
      ModelId  model_id{};
    };
  } // namespace avion::core::scene

#endif