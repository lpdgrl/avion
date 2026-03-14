#pragma once

#include <vector>
#include <algorithm>

#include "object.hpp"
#include "light.hpp"

namespace avion::core {

    enum class TypeObject {
        kLight = 2,
        kCube = 3,
        kPyramid = 4,
    };
    
    struct SceneObject {
        SceneObject(TypeObject type, int id, ObjectParams params);
        TypeObject type;
        Object object;
    };
    
    struct SceneLight {
        std::size_t id = 0;
        Light light;
        Color color;
        Size size;
    };

    std::string TypeObjectToString(TypeObject type);

    class Scene {
    public:
        using Objects = std::vector<SceneObject>;
        using SourceLight = std::vector<SceneLight>;

        Scene() = default;
        explicit Scene(size_t number_object);

        Scene(const Scene& scene) = delete;
        Scene(Scene&& scene) = delete;

        Scene& operator=(const Scene& scene) = delete;
        Scene& operator=(Scene&& scene) = delete;

        ~Scene();
        
        template <typename Params> 
        void AddObjectToScene(TypeObject type, Params params);
        
        SourceLight& GetAllSourceLights();
        Objects& GetAllObjects();
        
        size_t GetNumberObjects() const;
        size_t GetNumberSourceLights() const;

        Object* GetObject(int id);
        Object* GetObject(TypeObject type);
    
    private:
        void AddObject(TypeObject type, ObjectParams params);
        void AddSourceLight(LightParams params);

    private:
        Objects objects_on_scene_;
        SourceLight source_lights_on_scene_;
    };

    template <typename Params>
    void Scene::AddObjectToScene(TypeObject type, Params params)
    {
        // TODO: Or defining which call done by type of params by template magic (is_same)?? 
        if constexpr (std::is_same_v<Params, ObjectParams>) {
            AddObject(type, params);
        }  else if constexpr (std::is_same_v<Params, LightParams>) {
            AddSourceLight(params);
        } 
    }
} // namespace avion::core
