#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"
#include "AvionEngineCore/renderer/pipeline_queue.hpp"

namespace avion::core {  
  Scene::Scene(size_t number_objects, ResManager& resman, PipelineQueue& pl_queue) 
  : m_resman(resman)
  , m_pl_queue(pl_queue)
  {
      objects_on_scene_.reserve(number_objects);
      source_lights_on_scene_.reserve(number_objects);
      m_models.reserve(number_objects);
  }

  Scene::~Scene() 
  {
  AV_LOG_DEBUG("Scene is destroyed");
  }

  void Scene::AddObject(ObjectType type, ObjectParams params) {
      size_t n = objects_on_scene_.size() + 1;
      objects_on_scene_.emplace_back(n, type, params);
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
    std::size_t n = source_lights_on_scene_.size() + 1;

    source_lights_on_scene_.emplace_back( 
      MakeSourceLight(type),
      n,
      type,
      glm::vec3(0.471f),
      glm::vec3(0.25f));
  }

  Scene::SourceLight& Scene::GetAllSourceLights()
  {
      return source_lights_on_scene_;
  }

  Scene::Objects& Scene::GetAllObjects() {
      return objects_on_scene_;
  }

  std::size_t Scene::GetNumberObjects() const noexcept {
      return objects_on_scene_.size();
  }

  std::size_t Scene::GetNumberSourceLights() const noexcept
  {
      return source_lights_on_scene_.size();
  }

  std::size_t Scene::GetNumberModels() const noexcept
  {
    return m_models.size();
  }

  std::size_t Scene::GetAllNumberObjects() const noexcept
  {
    return GetNumberObjects() + GetNumberSourceLights() + GetNumberModels();
  }

  SceneObject* Scene::GetObject(int id) 
  {
    return const_cast<SceneObject*>(static_cast<const Scene*>(this)->GetObject(id));
  }

  Object* Scene::GetObject(ObjectType type) {
      auto it_object = std::find_if(objects_on_scene_.begin(), objects_on_scene_.end(), [&](SceneObject& obj_scene) {
          return obj_scene.type == type;
      });

      if (it_object == objects_on_scene_.end()) {
          return nullptr;
      }

      return
       &it_object->object;
  }

  SceneObject::SceneObject(std::uint16_t id, ObjectType type, ObjectParams params)
      : id(id)
      , type(type)
      , object(params)
  {}

  bool Scene::AddModel(const std::string& model_name) 
  {
    auto* p_res = m_resman.GetResource<resman::ResourceManager::FsPath>(model_name);
    if (p_res == nullptr)
    {
      AV_LOG_ERROR("Scene::AddModel");
      return false;
    }

    std::uint16_t id = static_cast<std::uint16_t>(m_models.size() + 1);

     // Если модель есть в кэше, то возвращаем на неё указатель
    if (auto *ptr = GetModelFromCache(model_name); ptr != nullptr)
    {
      m_models.emplace_back(std::make_unique<ModelHandler>(id, ptr->model));
      return true;
    }
    
    // Если в кеше нет модели, то грузим и ложим указатель в кэш
    // Good way :)
    auto& ptr = m_models.emplace_back(std::make_unique<ModelHandler>(id, p_res->parent_path(), p_res->filename(), m_resman));
    auto result = ptr->model.LoadModel();

    m_cache_models.emplace(model_name, ptr.get());

    // PIPELINE QUEUE????
    m_pl_queue.Enqueue(ptr->model.GetMeshs());

    return result;
  }

  ModelHandler* Scene::GetModelFromCache(const std::string& filename_model) noexcept
  {
    if (auto it_model = m_cache_models.find(filename_model); it_model != m_cache_models.cend())
    {
      return it_model->second;
    }

    return nullptr;
  }

  Scene::Models& Scene::GetModels()
  {
    return m_models;
  }

  const Scene::SourceLight& Scene::GetAllSourceLights() const noexcept
  {
    return source_lights_on_scene_;
  }

  const Scene::Objects& Scene::GetAllObjects() const noexcept
  {
    return objects_on_scene_;
  }

  const Scene::Models& Scene::GetModels() const noexcept
  {
    return m_models;
  }

  const SceneObject* Scene::GetObject(int id) const noexcept
  {
    auto it_object = std::find_if(objects_on_scene_.begin(), objects_on_scene_.end(), [&](const SceneObject& obj_scene) 
    {
        return obj_scene.id == id;
    });

    if (it_object == objects_on_scene_.end()) {
        return nullptr;
    }

    // Return pointer to Object from iterator
    return &(*it_object);
  }

  ModelHandler::ModelHandler(std::uint16_t id, const std::string& path, const std::string& filename, resman::ResourceManager& resman)
  : id(id)
  , model(path, filename, resman)
  {

  }

  ModelHandler::ModelHandler(std::uint16_t id, const avion::gfx::Model& model)
  : id(id)
  , model(model)
  {

  }
} // namespace avion::core
