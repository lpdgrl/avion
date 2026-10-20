#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <type_traits>
#include <deque>
#include <ranges>
#include <optional>
#include <cstdint>

#include "AvionEngineCore/core/object.hpp"
#include "AvionEngineCore/core/ISceneItem.hpp"
#include "AvionEngineCore/renderer/camera.hpp"

#include "AvionEngineCore/core/Common/CameraProxy.hpp"
#include "AvionEngineCore/core/Common/MiCounter.hpp"

#include "AvionEngineCore/core/ModelManager/ModelManager.hpp"

#include "AvionEngineCore/core/Serialization/Detail/EntitySerialize.hpp"

#include "AvionEngineCore/core/SceneSubsystem/Attribute.hpp"
#include "AvionEngineCore/core/SceneSubsystem/Entity.hpp"
#include "AvionEngineCore/core/LightSubsystem/Light.hpp"


namespace avion::core::detail
{
  // HELPERS FOR DEFINE ATTRIBUTE TYPE
  template <scene::Attribute A>
  struct AttributeType;

  template<>
  struct AttributeType<scene::Attribute::DirectionalLight>
  {
    using type = light::DirectionalLight;
  };

  template<>
  struct AttributeType<scene::Attribute::PointLight>
  {
    using type = light::PointLight;
  };

  template<>
  struct AttributeType<scene::Attribute::SpotLight>
  {
    using type = light::SpotLight;
  };

  template<scene::Attribute A>
  using AttributeType_t = typename AttributeType<A>::type;

  template <typename T>
  constexpr std::string TypeObjectToString(T type)
  { 
    using LightType = core::LightType;
  
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

// Forward declaration
namespace avion::core::resman
{
  class ResourceManager;
}

namespace avion::core {
  class Scene {
  public:
    using ModelManager  = modelmanager::ModelManager;
    using TextureManager = texturemanager::TextureManager;
    using SceneItems    = std::deque<std::unique_ptr<ISceneItem>>;
    using CacheItems    = std::unordered_map<std::uint32_t, ISceneItem&>;
    using CacheLightItems = std::vector<LightItem*>;
    using ResManager    = resman::ResourceManager;
    using Camera        = gfx::Camera;
    using CameraData    = gfx::CameraData;
    using CameraProxy   = common::CameraProxy;
    using PrimitiveType = modelmanager::detail::PrimitiveType;
    using EntitySerialize = serialization::detail::EntitySerialize;

    using ModelID            = std::uint64_t;
    using MiCounter          = common::MiCounter<scene::EntityID>;
    using EntityStorage      = std::vector<scene::Entity>;
    template <typename T>
    using EntityComponentMap = std::unordered_map<scene::EntityID, T>;

  private:
    MiCounter                                   m_mi_counter;
    EntityStorage                               m_entities;
    EntityComponentMap<ModelID>                 m_entity_models;
    EntityComponentMap<light::DirectionalLight> m_directional_lights;
    EntityComponentMap<light::PointLight>       m_point_lights;
    EntityComponentMap<light::SpotLight>        m_spot_lights;
    EntityComponentMap<gfx::Camera>             m_cameras;

    ModelManager& m_model_manager;
    TextureManager& m_texture_manager;
    SceneItems    m_storage_items;
    CacheItems    m_cache_items;
    CacheLightItems m_cache_light_items; 
    Camera        m_camera;
    std::uint32_t m_last_scene_item_id{};
    std::uint32_t m_number_point_light;
    std::uint32_t m_number_spot_light;

  public:
    Scene() = default;
    explicit Scene(size_t number_object, ModelManager& model_manager, TextureManager& texture_manager);

    Scene(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;

    Scene& operator=(const Scene& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;

    ~Scene();

    // ВОПРОС: КАК МНЕ ОПРЕДЕЛИТЬ, ЧТО НУЖНО СОЗДАТЬ ОСВЕЩЕНИЕ ИЛИ МОДЕЛЬ ИЛИ КАМЕРУ????
    // Для добавления сущности нужна политика выбора типа сущности??? 
    AV_ATTR_NODISCARD auto CreateEntity(std::string_view name, ModelID model_id) -> std::uint64_t;
    AV_ATTR_NODISCARD auto AddAttributeToEntity(scene::EntityID id, scene::Attribute attribute) -> bool;

    AV_ATTR_NODISCARD auto GetModelID(scene::EntityID id) const noexcept -> std::optional<ModelID>;

    AV_ATTR_NODISCARD decltype(auto) GetAttribute(scene::EntityID id, scene::Attribute attribute) const noexcept;
  
  
    AV_ATTR_NODISCARD auto AddItem(LightType type) -> bool;
    AV_ATTR_NODISCARD auto AddItem(const std::string& name) -> bool;
    AV_ATTR_NODISCARD auto AddItem(PrimitiveType type) -> bool;
    
    auto DeleteScene() noexcept -> void;
    auto DeleteItem(std::uint32_t id) noexcept -> bool;
    
    AV_ATTR_NODISCARD auto Import() noexcept -> bool;
    AV_ATTR_NODISCARD auto Export() const -> bool;

    SceneItems& GetSceneItems();
    const SceneItems&  GetSceneItems() const noexcept;
    std::optional<ISceneItem&> GetItem(std::uint32_t id) noexcept;

    CacheLightItems& GetCacheLightItems() noexcept { return m_cache_light_items; }
    std::uint32_t GetNumberPointLight() const noexcept { return m_number_point_light; }
    std::uint32_t GetNumberSpotLight() const noexcept { return m_number_spot_light; }

    // Camera interact 
    CameraData GetCameraData() const noexcept;
    CameraProxy& GetCameraProxy() noexcept { return m_camera.GetProxy(); }

  private:
    template <typename Attribute, typename ComponentMap>
    AV_ATTR_NODISCARD
    auto SettingAttribute(scene::EntityID id, ComponentMap& map) -> bool
    {
      Attribute attr;
      map.try_emplace(id, std::move(attr));
      return true;
    }

    template <typename ContainerType, typename ComponentMap = std::remove_const_t<const ContainerType>>
    AV_ATTR_NODISCARD
    decltype(auto) Find(scene::EntityID id, ComponentMap& map) const
    {
      using ReturnType = ComponentMap::mapped_type;
      std::optional<ReturnType> result;
      auto iter = map.find(id);
      if (iter == map.end())
      {
        AV_LOG_DEBUG("Scene::Find(EntityID, ComponentMap&): id = " + std::to_string(id) + " isn't exists!");
        return result;
      }
      result = iter->second;
      return result;
    }

    template <scene::Attribute attrib>
    AV_ATTR_NODISCARD
    auto GetAttributeType() const noexcept
    {  
      return std::type_identity<detail::AttributeType_t<attrib>>{};
    }
    
    // factory member func
    std::unique_ptr<ILight> MakeSourceLight(LightType type) const noexcept; 
    std::uint32_t IncAndGetId() noexcept { return ++m_last_scene_item_id; } 
  };


} // namespace avion::core
