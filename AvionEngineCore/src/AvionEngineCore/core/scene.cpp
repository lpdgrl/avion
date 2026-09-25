#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/Serialization/SceneSerialization.hpp"

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
        ItemType::kPrimitiveObject
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

  auto Scene::Export() const -> bool
  {
    serialization::detail::SceneSerialize scene_serialize;

    auto&& entities = scene_serialize.entities;
    std::vector<serialization::detail::DirLightSerialize> dir_light_entities;
    std::vector<serialization::detail::PointLightSerialize> point_light_entities;
    std::vector<serialization::detail::SpotLightSerialize> spot_light_entities;

    point_light_entities.reserve(m_number_point_light);
    spot_light_entities.reserve(m_number_spot_light);
    entities.reserve(m_storage_items.size() - (m_number_point_light + m_number_spot_light));
 
    for (const auto& item : m_storage_items)
    {
      if (item->item_type == ItemType::kObject || item->item_type == ItemType::kPrimitiveObject)
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

      else if (item->item_type == ItemType::kSourceLight)
      {
        auto&& transform = item->ptr_model->GetTransform();
        serialization::detail::Transform t
        {
          .position = {transform.position.x, transform.position.y, transform.position.z},
          .rotation = {transform.rotation.x, transform.rotation.y, transform.rotation.z},
          .size = {transform.size.x, transform.size.y, transform.size.z},
          .rotate_value = transform.value_rotate,
          .axis_rotate = static_cast<int>(transform.axis)
        };

        const auto* item_light = static_cast<LightItem*>(item.get());
        std::string filename(detail::TypeObjectToString(item_light->light_type));
        std::array<float, 3> ambient = item_light->light->GetAmbientArray();
        std::array<float, 3> diffuse = item_light->light->GetDiffuseArray();
        std::array<float, 3> specular = item_light->light->GetSpecularArray();

        if (item_light->light_type == LightType::kDirLight)
        {
          const auto* dir_light = static_cast<DirLight*>(item_light->light.get());
          auto direction = dir_light->GetDirection();
          serialization::detail::DirLightSerialize entity
          {
            .transform = t,
            .filename = filename,
            .direction = {direction.x, direction.y, direction.z},
            .ambient = ambient,
            .diffuse = diffuse,
            .specular = specular,
            .type = static_cast<std::uint8_t>(item_light->light_type)
          };
          dir_light_entities.push_back(entity);
        }

        else if (item_light->light_type == LightType::kPointLight)
        {
          const auto& point = static_cast<PointLight*>(item_light->light.get());
          auto position = point->GetGeometry();
          serialization::detail::PointLightSerialize entity
          {
            .transform = t,
            .filename = filename,
            .position = {position.x, position.y, position.z},
            .ambient = ambient,
            .diffuse = diffuse,
            .specular = specular,
            .constant = point->GetConstant(),
            .linear = point->GetLinear(),
            .quadratic = point->GetQuadratic(),
            .type = static_cast<std::uint8_t>(item_light->light_type)
          };
          point_light_entities.push_back(entity);
        }

        else if (item_light->light_type == LightType::kSpotLight)
        {
          const auto& spot = static_cast<SpotLight*>(item_light->light.get());
          auto position = spot->GetPosition();
          auto direction = spot->GetDirection();
          serialization::detail::SpotLightSerialize entity
          {
            .transform = t,
            .filename = filename,
            .position = {position.x, position.y, position.z},
            .direction = {direction.x, direction.y, direction.z},
            .ambient = ambient,
            .diffuse = diffuse,
            .specular = specular,
            .constant = spot->GetConstant(),
            .linear = spot->GetLinear(),
            .quadratic = spot->GetQuadratic(),
            .cutoff = spot->GetCutOff(),
            .outer_cutoff = spot->GetOuterCutOff(),
            .type = static_cast<std::uint8_t>(item_light->light_type)
          };
          spot_light_entities.push_back(entity);
        }
      }
    }
    scene_serialize.dir_light_entities = std::move(dir_light_entities);
    scene_serialize.point_light_entities = std::move(point_light_entities);
    scene_serialize.spot_light_entities = std::move(spot_light_entities);

    serialization::SceneSerialization::Save(scene_serialize);
    return true;
  }

  auto Scene::Import() noexcept -> bool
  {
    DeleteScene();

    auto scene = serialization::SceneSerialization::Load();
    auto&& entities = scene.entities;
    auto&& dir_light_entities = scene.dir_light_entities;
    auto&& point_light_entities = scene.point_light_entities;
    auto&& spot_light_entities = scene.spot_light_entities;

    auto transform_lambda = [](gfx::Transform& lhs, const serialization::detail::Transform& rhs)
    {
      lhs.position.x = rhs.position[0];
      lhs.position.y = rhs.position[1];
      lhs.position.z = rhs.position[2];

      lhs.rotation.x = rhs.rotation[0];
      lhs.rotation.y = rhs.rotation[1];
      lhs.rotation.z = rhs.rotation[2];

      lhs.size.x = rhs.size[0];
      lhs.size.y = rhs.size[1];
      lhs.size.z = rhs.size[2];

      lhs.value_rotate = rhs.rotate_value;
      lhs.axis = static_cast<gfx::AxisRotate>(rhs.axis_rotate);
    };

    auto array_to_vec3_light = [](const std::array<float, 3>& rhs) -> glm::vec3
    {
      return {rhs[0], rhs[1], rhs[2]};
    };

    auto array_to_vec3_pos = [](const std::array<float, 3>& rhs) -> glm::vec3
    {
      return {rhs[0], rhs[1], rhs[2]};
    };

    for (const auto& entity : entities)
    {
      if (static_cast<ItemType>(entity.type) == ItemType::kObject)
      {
        auto result = AddItem(entity.filename_model);
      }
      // else if (static_cast<ItemType>(entity.type) == ItemType::kPrimitiveObject)
      // {
      //   AddItem(static_cast<ItemType>(entity.type));
      // }
      
      auto& added_item = m_storage_items.back();
      auto&& transform = added_item->ptr_model->GetTransform();
      auto&& material = added_item->ptr_model->GetMaterial();

      transform_lambda(transform, entity.transform);

      material.shininess = entity.material.shininess;
      material.color.r = entity.material.color.red;
      material.color.g = entity.material.color.green;
      material.color.b = entity.material.color.blue;
    }

    // TODO: Work is terrible with items light!!!!!!!!
    if (dir_light_entities.has_value())
    {
      const auto& dir_lights = dir_light_entities.value();
      for (const auto& light_ser : dir_lights)
      {
        auto result = AddItem(static_cast<LightType>(light_ser.type));

        
        auto* last_light = static_cast<LightItem*>(m_storage_items.back().get());
        auto& transform = last_light->ptr_model->GetTransform();

        transform_lambda(transform, light_ser.transform);
        last_light->light->SetAmbient(array_to_vec3_light(light_ser.ambient));
        last_light->light->SetDiffuse(array_to_vec3_light(light_ser.diffuse));
        last_light->light->SetSpecular(array_to_vec3_light(light_ser.specular));
        auto* dir_light = static_cast<DirLight*>(last_light->light.get());

        dir_light->SetDirection(array_to_vec3_pos(light_ser.direction));
      }
    }

    if (point_light_entities.has_value())
    {
      const auto& point_lights = point_light_entities.value();
      for (const auto& light_ser : point_lights)
      {
        auto result = AddItem(static_cast<LightType>(light_ser.type));
        auto* last_light = static_cast<LightItem*>(m_storage_items.back().get());
        auto& transform = last_light->ptr_model->GetTransform();

        transform_lambda(transform, light_ser.transform);
        last_light->light->SetAmbient(array_to_vec3_light(light_ser.ambient));
        last_light->light->SetDiffuse(array_to_vec3_light(light_ser.diffuse));
        last_light->light->SetSpecular(array_to_vec3_light(light_ser.specular));
        auto* point = static_cast<PointLight*>(last_light->light.get());

        point->SetPosition(array_to_vec3_pos(light_ser.position));
        point->SetConstant(light_ser.constant);
        point->SetLinear(light_ser.linear);
        point->SetQuadratic(light_ser.quadratic);
      }
    }

    if (spot_light_entities.has_value())
    {
      const auto& spot_lights = spot_light_entities.value();
      for (const auto& serialize : spot_lights)
      {
        auto result = AddItem(static_cast<LightType>(serialize.type));
        auto* last_light = static_cast<LightItem*>(m_storage_items.back().get());
        auto& transform = last_light->ptr_model->GetTransform();

        transform_lambda(transform, serialize.transform);
        last_light->light->SetAmbient(array_to_vec3_light(serialize.ambient));
        last_light->light->SetDiffuse(array_to_vec3_light(serialize.diffuse));
        last_light->light->SetSpecular(array_to_vec3_light(serialize.specular));
        auto* spot = static_cast<SpotLight*>(last_light->light.get());

        spot->SetPosition(array_to_vec3_pos(serialize.position));
        spot->SetDirection(array_to_vec3_pos(serialize.direction));
        spot->SetConstant(serialize.constant);
        spot->SetLinear(serialize.linear);
        spot->SetQuadratic(serialize.quadratic);
        spot->SetCutOff(serialize.cutoff);
        spot->SetOuterCutOff(serialize.outer_cutoff);
      }
    }

    return true;
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
      if (item_type == ItemType::kObject || item_type == ItemType::kPrimitiveObject)
      {
        m_cache_items.erase(id);
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
          m_number_point_light--;
          m_number_spot_light--;
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
    m_number_point_light = 0;
    m_number_spot_light  = 0;

    m_storage_items.clear();
    m_cache_items.clear();
    m_cache_light_items.clear();
  }

} // namespace avion::core
