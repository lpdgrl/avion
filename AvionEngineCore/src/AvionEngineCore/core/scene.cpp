#include "../../../includes/AvionEngineCore/core/scene.hpp"

namespace avion::core {

   Scene::Scene(size_t number_objects) {
       objects_on_scene_.reserve(number_objects);
       source_lights_on_scene_.reserve(number_objects);
   }

   Scene::~Scene() {
       std::cout << "Scene is destroyed" << '\n';
   }

   void Scene::AddObject(TypeObject type, ObjectParams params) {
       size_t n = objects_on_scene_.size();
       objects_on_scene_.emplace_back(type, ++n, params);
   }

   void Scene::AddSourceLight(LightParams params) 
   {
       size_t n = source_lights_on_scene_.size();
       source_lights_on_scene_.emplace_back(++n, params.light, params.color, params.size);
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

    Object* Scene::GetObject(TypeObject type) {
        auto it_object = std::find_if(objects_on_scene_.begin(), objects_on_scene_.end(), [&](SceneObject& obj_scene) {
            return obj_scene.type == type;
        });

        if (it_object == objects_on_scene_.end()) {
            return nullptr;
        }

        return &it_object->object;
    }

    std::string TypeObjectToString(TypeObject type) {
        switch (type) {
            case TypeObject::kCube:
                return "Cube";
            case TypeObject::kLight:
                return "Light";
            case TypeObject::kPyramid:
                return "Pyramid";
        }
        return {};
    }

    SceneObject::SceneObject(TypeObject type, int id, ObjectParams params)
        : type(type)
        , object(id, params)
    {}

} // namespace avion::core
