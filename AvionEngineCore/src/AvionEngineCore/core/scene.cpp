#include "../../../includes/AvionEngineCore/core/scene.hpp"

namespace avion::core {  
   Scene::Scene(size_t number_objects) {
       objects_on_scene_.reserve(number_objects);
       source_lights_on_scene_.reserve(number_objects);
   }

   Scene::~Scene() {
       std::cout << "Scene is destroyed" << '\n';
   }

   void Scene::AddObject(ObjectType type, ObjectParams params) {
       size_t n = objects_on_scene_.size();
       objects_on_scene_.emplace_back(type, ++n, params);
   }

   void Scene::AddSourceLight(LightType type) 
   {
    std::size_t n = source_lights_on_scene_.size() + 1;
    switch(type)
    {
      case LightType::kDirLight:
      case LightType::kSimpleLight:
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
            glm::vec3(0.f),
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
    }
   }

    Scene::SourceLight& Scene::GetAllSourceLights() 
    {
        return source_lights_on_scene_;
    }

    Scene::Objects& Scene::GetAllObjects() {
        return objects_on_scene_;
    }

    size_t Scene::GetNumberObjects() const {
        return objects_on_scene_.size();
    }

    std::size_t Scene::GetNumberSourceLights() const 
    {
        return source_lights_on_scene_.size();
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

        return &it_object->object;
    }

    std::string TypeObjectToString(ObjectType type) {
        switch (type) {
            case ObjectType::kCube:
                return "Cube";
            // case ObjectType::kLight:
            //     return "Light";
            case ObjectType::kPyramid:
                return "Pyramid";
        }
        return {};
    }

    SceneObject::SceneObject(ObjectType type, int id, ObjectParams params)
        : type(type)
        , object(id, params)
    {}

} // namespace avion::core
