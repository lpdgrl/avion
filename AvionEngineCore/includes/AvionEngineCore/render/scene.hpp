#pragma once

#include <vector>

#include "object.hpp"

class Scene {
public:
    using Objects = std::vector<Object>;

    Scene() = default;
    Scene(size_t number_object);

    Scene(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;

    Scene& operator=(const Scene& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;

    ~Scene() = default;

    void AddObjectToScene(Position pos, Size sz);
    const Objects& GetAllObjects() const;
    size_t GetNumberObjects() const;

private:
    Objects objects_on_scene_;
};