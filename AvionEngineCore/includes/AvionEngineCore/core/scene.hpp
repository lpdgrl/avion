#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <type_traits>

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

  enum class ModelType
  {

  };
  
  struct SceneObject {
    SceneObject(std::uint16_t id, ObjectType type, ObjectParams params);

    std::uint16_t id{};
    ObjectType type;
    Object object;
  };
  
  struct SceneLight {
    std::unique_ptr<ILight> light;
    std::size_t id = 0;
    LightType type;
    Color color;
    Size size;
  };

  struct ModelHandler
  {
    ModelHandler() = delete;
    ModelHandler(std::uint16_t id, const std::string& path, const std::string& filename, resman::ResourceManager& resman);
    ModelHandler(std::uint16_t id, const avion::gfx::Model& model);

    std::uint16_t id = 0;
    avion::gfx::Model model;
  };

  class Scene {
  public:
    using Objects       = std::vector<SceneObject>;
    using SourceLight   = std::vector<SceneLight>;
    using Models        = std::vector<std::unique_ptr<ModelHandler>>;
    using ResManager    = resman::ResourceManager;
    using PipelineQueue = gfx::PipelineQueue;
    using ModelCache    = std::unordered_map<std::string, ModelHandler*>;

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

    const SourceLight&   GetAllSourceLights() const noexcept;
    const Objects& GetAllObjects() const noexcept;
    const Models&  GetModels() const noexcept;

    std::size_t GetNumberObjects() const noexcept;
    std::size_t GetNumberSourceLights() const noexcept;
    std::size_t GetNumberModels() const noexcept;

    std::size_t GetAllNumberObjects() const noexcept;

    Object*       GetObject(int id);
    Object*       GetObject(ObjectType type);
    SceneLight*   GetLight(int id);
    ModelHandler* GetModel(std::uint16_t id) noexcept;
    ModelHandler* GetModel(const std::string& filename);   
    ModelHandler* GetModel(std::uint16_t id, const std::string& model_name) noexcept;   

    const Object*       GetObject(int id) const noexcept;
    const SceneLight*   GetLight(int id) const noexcept;
    const ModelHandler* GetModel(std::uint16_t id) const noexcept;
    const ModelHandler* GetModel(const std::string& filename) const noexcept;
    const ModelHandler* GetModel(std::uint16_t id, const std::string& model_name) const noexcept;

  private:
    void AddObject(ObjectType type, ObjectParams params);
    std::unique_ptr<ILight> MakeSourceLight(LightType type) const noexcept;

    ModelHandler* GetModelFromCache(const std::string& filename_model) noexcept;

  private:
    Objects objects_on_scene_;
    SourceLight source_lights_on_scene_;
    Models m_models;
    ModelCache m_cache_models;
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


namespace avion::core::detail
{
  template <typename T>
  constexpr std::string TypeObjectToString(T type)
  { 
    using ObjectType = core::ObjectType;
    using LightType  = core::LightType;
  
    std::string result;
    if constexpr (std::is_same_v<T, ObjectType>)
    {
      switch(type)
      {
        case ObjectType::kCube:
        {
          result = "Cube";
          break;
        }
        case ObjectType::kPyramid:
        {
          result = "Pyramid";
          break;
        }
      }
    }

    if constexpr (std::is_same_v<T, LightType>)
    {
      switch(type)
      {
        case LightType::kDirLight:
        {
          result = "DirLight";
          break;
        }
        case LightType::kPointLight:
        {
          result = "PointLight";
          break;
        }
        case LightType::kSpotLight:
        {
          result = "SpotLight";
          break;
        }
        case LightType::kSimpleLight:
        {
          result = "SimpleLight";
          break;
        }
        case LightType::kUnknownLight:
        {
          result = "UnknownLight";
          break;
        }
      }
    }
    return result;
  }
} // namespace avion::core::detail