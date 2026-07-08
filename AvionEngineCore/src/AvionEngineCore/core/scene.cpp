#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/renderer/pipeline_queue.hpp"

namespace avion::core {  
  Scene::Scene(size_t number_objects, ModelManager& model_manager) 
  : m_model_manager(model_manager)
  , m_camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f))
  {
    source_lights_on_scene_.reserve(number_objects);
    // m_models.reserve(number_objects);
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
    std::size_t n = ++m_last_scene_item_id;
    auto&& result = m_model_manager.Load("light-bulb-color-icon.png", ModelManager::PrimitiveType::kPlane);
    if (!result.has_value())
    {
      AV_LOG_INFO("Scene::AddSourceLight: result from model manager is empty!");
      return;
    }

    auto&& model_item = result.value();
    source_lights_on_scene_.emplace_back( 
      MakeSourceLight(type),
      n,
      type,
      std::move(model_item.model_handler),
      std::move(model_item.model)
      // glm::vec3(0.471f),
      // glm::vec3(0.25f)
    );
  }

  Scene::SourceLight& Scene::GetAllSourceLights()
  {
      return source_lights_on_scene_;
  }

  std::size_t Scene::GetNumberSourceLights() const noexcept
  {
      return source_lights_on_scene_.size();
  }

  // std::size_t Scene::GetNumberModels() const noexcept
  // {
  //   return m_models.size();
  // }

  bool Scene::AddModel(const std::string& model_name) 
  {
    // auto* p_res = m_resman.GetResource<resman::ResourceManager::FsPath>(model_name);
    // if (p_res == nullptr)
    // {
    //   AV_LOG_ERROR("Scene::AddModel: model not found.");
    //   return false;
    // }

    // std::uint16_t id = static_cast<std::uint16_t>(m_models.size() + 1);

    //  // Если модель есть в кэше, то возвращаем на неё указатель
    // if (auto *ptr = GetModelFromCache(model_name); ptr != nullptr)
    // {
    //   m_models.emplace_back(std::make_unique<ModelHandler>(id, ptr->model));
    //   return true;
    // }

    auto&& model_load_result = m_model_manager.Load(model_name);
    if (!model_load_result.has_value())
    {
      return false;
    }
    auto&& model_item = model_load_result.value();

    m_storage_items.emplace_back(++m_last_scene_item_id, std::move(model_item.model_handler), std::move(model_item.model));
    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item.id, ref_last_item);
    
    // Если в кеше нет модели, то грузим файл модели в импортер и кладём указатель в кэш
    // Good way :)d
    // auto& ptr = m_models.emplace_back(std::make_unique<ModelHandler>(id, p_res->parent_path(), p_res->filename(), m_resman));
    // auto result = ptr->model.LoadModel();

    // m_cache_models.emplace(model_name, ptr.get());
  
    // // PIPELINE QUEUE????
    // // m_pl_queue.Enqueue(ptr->model.GetMeshs());

    // return result;
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
    m_storage_items.emplace_back(++m_last_scene_item_id, std::move(model_item.model_handler), std::move(model_item.model));
    auto& ref_last_item = m_storage_items.back();
    m_cache_items.emplace(ref_last_item.id, ref_last_item);
    return true;
  }

  // ModelHandler* Scene::GetModelFromCache(const std::string& filename_model) noexcept
  // {
  //   if (auto it_model = m_cache_models.find(filename_model); it_model != m_cache_models.cend())
  //   {
  //     return it_model->second;
  //   }

  //   return nullptr;
  // }

  Scene::SceneItem& Scene::GetItem(std::uint32_t id) noexcept
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

  const Scene::SourceLight& Scene::GetAllSourceLights() const noexcept
  {
    return source_lights_on_scene_;
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
