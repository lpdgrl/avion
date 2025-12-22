#include "../../../includes/AvionEngineCore/render/scene.hpp"

Scene::Scene(size_t number_objects) {
    objects_on_scene_.reserve(number_objects);
}

void Scene::AddObjectToScene(Position pos, Size sz) {
    objects_on_scene_.emplace_back(pos, sz);
}

const Scene::Objects& Scene::GetAllObjects() const {
    return objects_on_scene_;
}

size_t Scene::GetNumberObjects() const {
    return objects_on_scene_.size();
}
