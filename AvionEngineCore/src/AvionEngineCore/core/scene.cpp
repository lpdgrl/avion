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

  [[nodiscard]] auto Scene::AddItem(LightType type) -> bool
  {
    auto&& result = m_model_manager.Load("light-bulb-color-icon.png", ModelManager::PrimitiveType::kPlane);
    if (!result.has_value())
    {
      AV_LOG_INFO("Scene::AddSourceLight: result from model manager is empty!");
      return false;
    }

    auto&& model_item = result.value();

    m_storage_items.emplace_back( 
      std::make_unique<LightItem>
      (
        std::move(model_item.gpu_buffer_handle), 
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
    return true;
  }

  [[nodiscard]] auto Scene::AddItem(const std::string& name) -> bool
  {
    auto&& model_load_result = m_model_manager.Load(name);
    if (!model_load_result.has_value())
    {
      return false;
    }
    auto&& model_item = model_load_result.value();
    
    m_storage_items.emplace_back
    (
      std::make_unique<ObjectItem>
      (
        std::move(model_item.gpu_buffer_handle), 
        std::move(model_item.model), 
        IncAndGetId(),
        ItemType::kObject
      )
    );

    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item->id, *ref_last_item.get());
    return true;
  }

  [[nodiscard]] auto Scene::AddItem(PrimitiveType type) -> bool
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
        std::move(model_item.gpu_buffer_handle), 
        std::move(model_item.model), 
        ++m_last_scene_item_id, 
        ItemType::kObject
      )
    );

    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item->id, *ref_last_item.get());
    return true;
  }

  std::optional<ISceneItem&> Scene::GetItem(std::uint32_t id) noexcept
  {
    std::optional<ISceneItem&> result;
    auto it = m_cache_items.find(id);
    if (it == m_cache_items.end())
    {
      AV_LOG_ERROR("Scene::GetItem(std::uint32_t id): id isn't exist!");
      return std::nullopt;
    }
    result = it->second;
    return result;
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

  auto Scene::Export() const -> std::vector<EntitySerialize>
  {
    std::vector<EntitySerialize> entities;
    // TODO: In future need add optimization
    // entities.reserve()

    for (const auto& item : m_storage_items)
    {
      if (item->item_type == ItemType::kObject)
      {
        auto& transform = item->ptr_model->GetTransform();
        auto& material = item->ptr_model->GetMaterial();

        EntitySerialize entity
        {
          .transform
          {
            .position = {transform.position.x, transform.position.y, transform.position.z},
            .rotation = {transform.rotation.x, transform.rotation.y, transform.rotation.z},
            .size = {transform.size.x, transform.size.y, transform.size.z},
            .rotate_value = transform.value_rotate,
            .axis_rotate = static_cast<int>(transform.axis)
          }, 
          .material 
          {
            .color
            {
              .red = material.color.r,
              .green = material.color.g,
              .blue = material.color.b,
            },
            .shininess = material.shininess,
            .material_type = static_cast<std::uint8_t>(material.type)
          },
          .filename_model = item->ptr_model->GetFileName(),
          .type = static_cast<std::uint8_t>(item->item_type)
        };
        entities.push_back(entity);
      }
    }
    return entities;
  }

  auto Scene::Import(const std::vector<EntitySerialize>& entities) noexcept -> void
  {
    if (entities.empty())
    {
      AV_LOG_ERROR("Scene::Import(const std::vector<EntitySerialize>& entities): vector of entities is empty!");
      return;
    }

    for (const auto& entity : entities)
    {
      AddItem(entity.filename_model);
      auto& added_item = m_storage_items.back();
      auto&& transform = added_item->ptr_model->GetTransform();
      auto&& material = added_item->ptr_model->GetMaterial();
      transform.position.x = entity.transform.position[0];
      transform.position.y = entity.transform.position[1];
      transform.position.z = entity.transform.position[2];

      transform.rotation.x = entity.transform.rotation[0];
      transform.rotation.y = entity.transform.rotation[1];
      transform.rotation.z = entity.transform.rotation[2];

      transform.size.x = entity.transform.size[0];
      transform.size.y = entity.transform.size[1];
      transform.size.z = entity.transform.size[2];

      transform.value_rotate = entity.transform.rotate_value;
      transform.axis = static_cast<gfx::AxisRotate>(entity.transform.axis_rotate);

      material.shininess = entity.material.shininess;
      material.color.r = entity.material.color.red;
      material.color.g = entity.material.color.green;
      material.color.b = entity.material.color.blue;
    }
  }

  auto Scene::DeleteItem(std::uint32_t id) noexcept -> bool
  {
    auto it_item = std::ranges::find_if(m_storage_items, 
      [&id](const std::unique_ptr<ISceneItem>& item)
      {
        return id == item->id;
      }
    );

    if (it_item != m_storage_items.end())
    { 
      auto& item = (*it_item);
      auto item_type = item->item_type;

      // Delete item from storage 
      m_storage_items.erase(it_item);

      // Delete item from cache
      if (item_type == ItemType::kObject)
      {
        m_cache_items.erase(id);
        m_last_scene_item_id--;
      }
      // Delete light item from cache
      else if (item_type == ItemType::kSourceLight)
      {
        auto it_light_item = std::ranges::find_if(m_cache_light_items, 
          [&id](const LightItem* item)
          {
            return id == item->id;
          }
        );
        if (it_light_item != m_cache_light_items.end())
        {
          m_cache_light_items.erase(it_light_item);
          m_number_point_light = 0;
          m_number_spot_light = 0;
          m_last_scene_item_id--;
        }
      }
      AV_LOG_INFO(std::format("Scene::DeleteItem(std::uint32_t id): The item with id = {} is deleted", id));
      return true;
    }
    AV_LOG_ERROR(std::format("Scene::DeleteItem(std::uint32_t id): id = {} isn't exist", id));
    return false;
  }

  auto Scene::DeleteScene() noexcept -> void
  {
    // TODO: Is it good?
    m_last_scene_item_id = 0;
    m_storage_items.clear();
    m_cache_items.clear();
    m_cache_light_items.clear();
  }

} // namespace avion::core
