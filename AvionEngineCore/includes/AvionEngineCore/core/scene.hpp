#pragma once

#include <vector>
#include <algorithm>

#include "object.hpp"
#include "light.hpp"

#include "AvionEngineCore/renderer/model.hpp"

// Forward declaration
namespace avion::core::resman
{
  class ResourceManager;
}

namespace avion::gfx
{
  class PipelineQueue;
}

namespace avion::core {
  
  enum class LightType {
      kUnknownLight = -1,
      kSimpleLight = 0,
      kDirLight = 1,
      kPointLight = 2,
      kSpotLight = 3,
  };

  enum class ObjectType {
      kCube = 3,
      kPyramid = 4,
  };
  
  struct SceneObject {
      SceneObject(ObjectType type, int id, ObjectParams params);
      ObjectType type;
      Object object;
  };
  
  struct SceneLight {
      std::unique_ptr<ILight> light;
      std::size_t id = 0;
      LightType type_light;
      Color color;
      Size size;
  };

  std::string TypeObjectToString(ObjectType type);

  class Scene {
  public:
    using Objects       = std::vector<SceneObject>;
    using SourceLight   = std::vector<SceneLight>;
    using Models        = std::vector<avion::gfx::Model>;
    using ResManager    = resman::ResourceManager;
    using PipelineQueue = gfx::PipelineQueue;

    Scene() = default;
    explicit Scene(size_t number_object, ResManager& resman, PipelineQueue& pl_queue);

    Scene(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;

    Scene& operator=(const Scene& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;

    ~Scene();
    
    template <typename Params> 
    void AddObjectToScene(ObjectType type, Params params);
    
    void AddSourceLight(LightType type);
    bool AddModel(const std::string& name_model);

    SourceLight& GetAllSourceLights();
    Objects& GetAllObjects();
    Models& GetModels();

    size_t GetNumberObjects() const;
    size_t GetNumberSourceLights() const;

    Object* GetObject(int id);
    Object* GetObject(ObjectType type);
    SceneLight* GetLight(int id);

  private:
    void AddObject(ObjectType type, ObjectParams params);

  private:
    Objects objects_on_scene_;
    SourceLight source_lights_on_scene_;
    Models m_models;
    ResManager& m_resman;
    PipelineQueue& m_pl_queue;
  };

  template <typename Params>
  void Scene::AddObjectToScene(ObjectType type, Params params)
  {
    // TODO: Or defining which call done by type of params by template magic (is_same)?? 
    if constexpr (std::is_same_v<Params, ObjectParams>) {
        AddObject(type, params);
    } 
  }
} // namespace avion::core
