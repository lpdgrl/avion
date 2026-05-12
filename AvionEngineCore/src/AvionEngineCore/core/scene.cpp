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
      size_t n = objects_on_scene_.size();
      objects_on_scene_.emplace_back(type, ++n, std::move(params));
  }

  void Scene::AddSourceLight(LightType type) 
  {
  std::size_t n = source_lights_on_scene_.size() + 1;
  switch(type)
  {
    case LightType::kDirLight:
    {
      // TODO: Put it in a separate method MakeDirLight
      source_lights_on_scene_.emplace_back( 
        std::make_unique<DirLight>(
          glm::vec3(0.f), 
          glm::vec3(1.f), 
          glm::vec3(1.f), 
          glm::vec3(1.f)),
        n,
        type,
        glm::vec3(1.f),
        glm::vec3(0.25f));
      break;
    }
    case LightType::kPointLight:
    {
      // TODO: Put it in a separate method MakePointLight
      source_lights_on_scene_.emplace_back(
        std::make_unique<PointLight>(
          glm::vec3(1.f),
          glm::vec3(1.f),
          glm::vec3(1.f),
          glm::vec3(1.f),
          1.f,
          0.09f,
          0.032f
          ),
        n,
        type,
        glm::vec3(1.f),
        glm::vec3(0.25));
      break;
    }

    case LightType::kSpotLight:
    {
      
      source_lights_on_scene_.emplace_back(
          std::make_unique<SpotLight>(
              glm::vec3(0.f),
              glm::vec3(0.f),
              glm::vec3(1.f),
              glm::vec3(1.f),
              glm::vec3(1.f),
              1.f,
              0.09f,
              0.032,
              0.f,
              0.f
            ),
          n,
          type,
          glm::vec3(1.f),
          glm::vec3(0.25));
      break;
    }
  }
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

  Scene::Model* Scene::GetModel(const std::string& filename)
  {
    auto it_model = std::find_if(m_models.begin(), m_models.end(), [&](Model& model) {
        return model.GetFileName() == filename;
    });

    if (it_model == m_models.end()) {
        return nullptr;
    }

    return &(*it_model);
  }


  Object* Scene::GetObject(int id) {
      auto it_object = std::find_if(objects_on_scene_.begin(), objects_on_scene_.end(), [&](SceneObject& obj_scene) {
          return obj_scene.object.GetId() == id;
      });

      if (it_object == objects_on_scene_.end()) {
          return nullptr;
      }

      // Return pointer to Object from iterator
      return &it_object->object;
  }

  SceneLight* Scene::GetLight(int id) {
    auto it_light = std::find_if(source_lights_on_scene_.begin(), source_lights_on_scene_.end(), 
        [&](SceneLight& light) {
          return light.id == id;
        });
    
    if (it_light == source_lights_on_scene_.end()) {
      return nullptr;
    }

    return &(*it_light);
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

  SceneObject::SceneObject(ObjectType type, int id, ObjectParams params)
      : type(type)
      , object(id, params)
  {}

  bool Scene::AddModel(const std::string& model_name) 
  {
    auto* p_res = m_resman.GetResource<resman::ResourceManager::FsPath>(model_name);
    if (p_res == nullptr)
    {
      AV_LOG_ERROR("Scene::AddModel");
      return false;
    }

    auto& ref = m_models.emplace_back(p_res->parent_path(), p_res->filename(), m_resman);
    auto result = ref.LoadModel();

    // PIPELINE QUEUE????
    m_pl_queue.Enqueue(ref.GetMeshs());

    return result;
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

  const Object* Scene::GetObject(int id) const noexcept
  {
    const Object* p_object = nullptr;
    p_object = GetObject(id);

    return p_object;
  }

  const SceneLight* Scene::GetLight(int id) const noexcept
  {
    const SceneLight* p_light = nullptr;
    p_light = GetLight(id);

    return p_light;
  }

  const Scene::Model* Scene::GetModel(const std::string& filename) const noexcept
  {
    const Model* p_model = nullptr;
    p_model = GetModel(filename);

    return p_model;
  }

} // namespace avion::core
