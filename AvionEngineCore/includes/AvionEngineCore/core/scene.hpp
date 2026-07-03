#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
#include <deque>

#include "AvionEngineCore/core/object.hpp"
#include "AvionEngineCore/core/light.hpp"
#include "AvionEngineCore/renderer/camera.hpp"
#include "AvionEngineCore/core/Common/CameraProxy.hpp"

#include "AvionEngineCore/core/ModelManager/ModelManager.hpp"

// Forward declaration
namespace avion::core::resman
{
  class ResourceManager;
}

namespace avion::core {
  
  enum class LightType {
    kUnknownLight = -1,
    kSimpleLight = 0,
    kDirLight = 1,
    kPointLight = 2,
    kSpotLight = 3,
  };

  struct SceneLight {
    std::unique_ptr<ILight> light;
    std::size_t id = 0;
    LightType type;

    bool is_selectable = false;
  };

  class Scene {
  public:
    using ModelManager  = modelmanager::ModelManager;
    using SourceLight   = std::vector<SceneLight>;
   
    struct SceneItem 
    {
      std::uint32_t id;
      ModelManager::ModelHandler model_handler;
      ModelManager::ModelPtr     p_model; 
    }; 

    using SceneItems    = std::deque<SceneItem>;
    using CacheItems    = std::unordered_map<std::uint32_t, SceneItem&>;

    using ResManager    = resman::ResourceManager;

    using Camera        = gfx::Camera;
    using CameraData    = gfx::CameraData;
    using CameraProxy   = common::CameraProxy;

    using PrimitiveType = modelmanager::detail::PrimitiveType;

    Scene() = default;
    explicit Scene(size_t number_object, ModelManager& model_manager);

    Scene(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;

    Scene& operator=(const Scene& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;

    ~Scene();
    
    void AddSourceLight(LightType type);
    bool AddModel(const std::string& name_model);
    bool AddPrimitive(PrimitiveType type); 

    SourceLight& GetAllSourceLights();
    SceneItems& GetSceneItems();

    SceneItem& GetItem(std::uint32_t id) noexcept;

    const SourceLight& GetAllSourceLights() const noexcept;
    const SceneItems&  GetSceneItems() const noexcept;

    std::size_t GetNumberSourceLights() const noexcept;
    // std::size_t GetNumberModels() const noexcept;

    template<typename Self>
    decltype(auto) GetLight(this Self& self, int id);

    // Camera interact 
    CameraData GetCameraData() const noexcept;
    CameraProxy& GetCameraProxy() noexcept { return m_camera.GetProxy(); }

    // template <typename Self>
    // decltype(auto) GetModel(this Self& self, std::uint32_t id, const std::string& filename);
    // template <typename Self>
    // decltype(auto) GetModel(this Self& self, std::uint32_t id);
    // template <typename Self>
    // decltype(auto) GetModel(this Self& self, const std::string& filename);

  private:
    // factory member func
    std::unique_ptr<ILight> MakeSourceLight(LightType type) const noexcept; 
    // ModelHandler*           GetModelFromCache(const std::string& filename_model) noexcept;

    // template <typename Self, typename Pred>
    // decltype(auto) FindModel(this Self& self, Pred&& pred);

  private:
    ModelManager& m_model_manager;
    SourceLight   source_lights_on_scene_;
    SceneItems  m_storage_items;
    CacheItems  m_cache_items;
    Camera m_camera;
    std::uint32_t m_last_scene_item_id{};

    // Models          m_models;
    // ModelCache      m_cache_models;
  };

  template<typename Self>
  decltype(auto) Scene::GetLight(this Self& self, int id)
  {
    using ReturnType = std::conditional_t<
      std::is_const_v<std::remove_reference_t<Self>>,
      const SceneLight*,
      SceneLight*
    >;

    auto it = std::find_if(self.source_lights_on_scene_.begin(), self.source_lights_on_scene_.end(), [id](const auto& handler)
      { return handler.id == id; });

    if (it == self.source_lights_on_scene_.end()) 
    {
      return ReturnType{nullptr};
    }

    return static_cast<ReturnType>(&(*it));
  }
  
  // template <typename Self>
  // decltype(auto) Scene::GetModel(this Self& self, std::uint32_t id, const std::string& filename)
  // {
  //   return self.FindModel([id, &filename] (const auto& handler) { return handler->id == id && handler->model.GetFileName() == filename; });
  // }

  // template <typename Self>
  // decltype(auto) Scene::GetModel(this Self& self, std::uint32_t id)
  // {
  //   return self.FindModel([id] (const auto& handler) { return handler->id == id; });
  // }

  // template <typename Self>
  // decltype(auto) Scene::GetModel(this Self& self, const std::string& filename)
  // {
  //   return self.FindModel([&filename] (const auto& handler) { return handler->model.GetFileName() == filename; });
  // }

  // template <typename Self, typename Pred>
  // decltype(auto) Scene::FindModel(this Self& self, Pred&& pred)
  // {
  //   using ReturnType = std::conditional_t<
  //     std::is_const_v<std::remove_reference_t<Self>>,
  //     const ModelHandler*,
  //     ModelHandler*
  //   >;

  //   auto it = std::find_if(self.m_models.begin(), self.m_models.end(), std::forward<Pred>(pred));

  //   if (it == self.m_models.end())
  //   {
  //     return ReturnType{nullptr};
  //   }

  //   return static_cast<ReturnType>(it->get());
  // }

} // namespace avion::core

namespace avion::core::detail
{
  template <typename T>
  constexpr std::string TypeObjectToString(T type)
  { 
    using LightType  = core::LightType;
  
    std::string result;
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