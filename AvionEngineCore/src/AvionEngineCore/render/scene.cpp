#include "../../../includes/AvionEngineCore/render/scene.hpp"

Scene::Scene(size_t number_objects) {
    objects_on_scene_.reserve(number_objects);
}

Scene::~Scene() {
    std::cout << "Scene is destroyed" << '\n';
}

void Scene::AddObjectToScene(Position pos, Size sz, Color color) {
    size_t n = objects_on_scene_.size();
    objects_on_scene_.emplace_back(++n, pos, sz, color);
}

Scene::Objects& Scene::GetAllObjects() {
    return objects_on_scene_;
}

size_t Scene::GetNumberObjects() const {
    return objects_on_scene_.size();
}
