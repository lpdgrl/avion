#include "../../../includes/AvionEngineCore/core/scene.hpp"

namespace avion::core {

    Scene::Scene(size_t number_objects) {
        objects_on_scene_.reserve(number_objects);
    }

    Scene::~Scene() {
        std::cout << "Scene is destroyed" << '\n';
    }

    void Scene::AddObjectToScene(TypeObject type, Position pos, Size sz, Color color) {
        size_t n = objects_on_scene_.size();
        objects_on_scene_.emplace_back(type, ++n, pos, sz, color);
    }

    Scene::Objects& Scene::GetAllObjects() {
        return objects_on_scene_;
    }

    size_t Scene::GetNumberObjects() const {
        return objects_on_scene_.size();
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

    SceneObject::SceneObject(TypeObject type, int id, Position position, Size size, Color color, Color mixing_color): type(type), object(id, position, size, color, mixing_color) {}

} // namespace avion::core