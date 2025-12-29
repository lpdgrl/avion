#pragma once

#include <vector>

#include "object.hpp"

enum class TypeObject {
    kCube = 0,
};

class Scene {
public:
    using Objects = std::vector<Object>;

    Scene() = default;
    Scene(size_t number_object);

    Scene(const Scene& scene) = delete;
    Scene(Scene&& scene) = delete;

    Scene& operator=(const Scene& scene) = delete;
    Scene& operator=(Scene&& scene) = delete;

    ~Scene();

    void AddObjectToScene(Position pos, Size sz, Color color);
    Objects& GetAllObjects() ;
    size_t GetNumberObjects() const;

private:
    Objects objects_on_scene_;
};