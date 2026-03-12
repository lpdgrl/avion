
#ifndef AVION_RENDERER_MATERIAL_H 
#define AVION_RENDERER_MATERIAL_H

    #include "glm/glm.hpp"
    #include <vector>

    namespace avion::gfx::material {

        struct Material {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;
        };
        
        constexpr Material Emerald_m {
            .ambient = glm::vec3(0.0215f, 0.1745, 0.0215),
            .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
            .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
            .shininess = 128 * 0.6 
        };
        
        constexpr Material Gold_m {
            .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
            .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
            .specular = glm::vec3(0.628281f, 0.555802f, 0.366065f),
            .shininess = 128 * 0.4
        };

        constexpr Material BlackPlastic_m {
            .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
            .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
            .specular = glm::vec3(0.50f, 0.50f, 0.50f),
            .shininess = 128 * 0.25
        };

    } // namespace avion::gfx::material

#endif // AVION_RENDERER_MATERIAL_H 

