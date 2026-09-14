#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/renderer/pipeline_queue.hpp"

namespace avion::core {  
  Scene::Scene(size_t number_objects, ModelManager& model_manager) 
  : m_model_manager(model_manager)
  , m_camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f))
  {
  }

  Scene::~Scene() 
  {
  AV_LOG_INFO("Scene::~Scene(): scene is destroyed");
  }

  std::unique_ptr<ILight> Scene::MakeSourceLight(LightType type) const noexcept
  {
    std::unique_ptr<ILight> ptr;

    switch (type)
    {
      case LightType::kDirLight:
      {
        ptr = std::make_unique<DirLight>(
          glm::vec3{0.f}, // direction
          glm::vec3{1.f}, // ambient
          glm::vec3{1.f}, // diffuse
          glm::vec3{1.f}  // specular
        );
        break;
      }
      case LightType::kPointLight:
      {
        ptr = std::make_unique<PointLight>(
          glm::vec3(1.f), // position
          glm::vec3(1.f), // ambient
          glm::vec3(1.f), // diffuse
          glm::vec3(1.f), // specular
          1.f,            // float constant
          0.09f,          // float linear
          0.032f          // float quadratic
        );
        break;
      }
      case LightType::kSpotLight:
      {
        ptr = std::make_unique<SpotLight>(
          glm::vec3(0.f), // position
          glm::vec3(0.f), // direction
          glm::vec3(1.f), // ambient
          glm::vec3(1.f), // diffuse
          glm::vec3(1.f), // specular
          1.f,            // float constant
          0.09f,          // float linear
          0.032,          // float quadratic
          0.f,            // float cutoff
          0.f             // float outercutoff  
        );
        break;
      }
    }
    return ptr;
  }

  void Scene::AddSourceLight(LightType type) 
  {
    auto&& result = m_model_manager.Load("light-bulb-color-icon.png", ModelManager::PrimitiveType::kPlane);
    if (!result.has_value())
    {
      AV_LOG_INFO("Scene::AddSourceLight: result from model manager is empty!");
      return;
    }

    auto&& model_item = result.value();

    m_storage_items.emplace_back( 
      std::make_unique<LightItem>
      (
        std::move(model_item.model_handler), 
        std::move(model_item.model), 
        IncAndGetId(),
        ItemType::kSourceLight,
        MakeSourceLight(type),
        type
      )
    );

    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item->id, *ref_last_item.get());
    auto* light_item = static_cast<LightItem*>(ref_last_item.get());

    m_cache_light_items.emplace_back(light_item);
    
    switch(type)
    {
      case LightType::kPointLight:
      {
        m_number_point_light++;
        break;
      }
      case LightType::kSpotLight:
      {
        m_number_spot_light++;
        break;
      }
    }
  }

  bool Scene::AddModel(const std::string& model_name) 
  {
    auto&& model_load_result = m_model_manager.Load(model_name);
    if (!model_load_result.has_value())
    {
      return false;
    }
    auto&& model_item = model_load_result.value();
    
    m_storage_items.emplace_back
    (
      std::make_unique<ObjectItem>
      (
        std::move(model_item.model_handler), 
        std::move(model_item.model), 
        IncAndGetId(),
        ItemType::kObject
      )
    );

    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item->id, *ref_last_item.get());
    return true;
  }

  bool Scene::AddPrimitive(PrimitiveType type)
  {
    auto&& primitive_model_load_result = m_model_manager.Load(type);
    if (!primitive_model_load_result.has_value())
    {
      return false;
    }

    auto&& model_item = primitive_model_load_result.value();
    m_storage_items.emplace_back
    (
      std::make_unique<ObjectItem>
      (
        std::move(model_item.model_handler), 
        std::move(model_item.model), 
        ++m_last_scene_item_id, 
        ItemType::kObject
      )
    );

    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item->id, *ref_last_item.get());
    return true;
  }

  ISceneItem& Scene::GetItem(std::uint32_t id) noexcept
  {
    auto it = m_cache_items.find(id);
    if (it == m_cache_items.end())
    {
      AV_LOG_ERROR("Scene::GetItem(std::uint32_t id): id isn't exist!");
      std::terminate();
    }
    return it->second;
  }

  Scene::SceneItems& Scene::GetSceneItems()
  {
    return m_storage_items;
  }

  const Scene::SceneItems& Scene::GetSceneItems() const noexcept
  {
    return m_storage_items;
  }

  Scene::CameraData Scene::GetCameraData() const noexcept
  {
    CameraData data
    {
      .view_matrix = m_camera.GetViewMatrix(),
      .position = m_camera.GetPosition()
    };
    return data;
  }

} // namespace avion::core
