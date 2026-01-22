#pragma once

#include <vector>
#include <algorithm>

#include "object.hpp"

namespace avion::core {

    enum class TypeObject {
        kLight = 2,
        kCube = 3,
        kPyramid = 4,
    };

    struct SceneObject {
        SceneObject(TypeObject type, int id, Position position, Size size, Color color, Color mixing_color = core::kDefMixColor);
        TypeObject type;
        Object object;
    };

    std::string TypeObjectToString(TypeObject type);

    class Scene {
    public:
        using Objects = std::vector<SceneObject>;

        Scene() = default;
        explicit Scene(size_t number_object);

        Scene(const Scene& scene) = delete;
        Scene(Scene&& scene) = delete;

        Scene& operator=(const Scene& scene) = delete;
        Scene& operator=(Scene&& scene) = delete;

        ~Scene();

        void AddObjectToScene(TypeObject type_object, Position pos, Size sz, Color color);
        Objects& GetAllObjects();
        size_t GetNumberObjects() const;
        Object* GetObject(int id);
        Object* GetObject(TypeObject type);

    private:
        Objects objects_on_scene_;
    };

} // namespace avion::core