
#ifndef AVION_CORE_MATERIAL_H 
#define AVION_CORE_MATERIAL_H

    #include "glm/glm.hpp"

    namespace avion::core {
        enum class PrefabMaterial {
            kUnknownMat     = 0,
            kEmerald        = 1,
            kGold           = 2,
            kBlackPlastic   = 3,
        };

        struct Material {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            float shininess;
        };
        
        static constexpr Material prefab_Emerald {
            .ambient = glm::vec3(0.0215f, 0.1745, 0.0215),
            .diffuse = glm::vec3(0.07568f, 0.61424f, 0.07568f),
            .specular = glm::vec3(0.633f, 0.727811f, 0.633f),
            .shininess = 128 * 0.6 
        };
        
        static constexpr Material prefab_Gold {
            .ambient = glm::vec3(0.24725f, 0.1995f, 0.0745f),
            .diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f),
            .specular = glm::vec3(0.628281f, 0.555802f, 0.366065f),
            .shininess = 128 * 0.4
        };

        static constexpr Material prefab_BlackPlastic {
            .ambient = glm::vec3(0.0f, 0.0f, 0.0f),
            .diffuse = glm::vec3(0.01f, 0.01f, 0.01f),
            .specular = glm::vec3(0.50f, 0.50f, 0.50f),
            .shininess = 128 * 0.25
        };
        
    } // namespace avion::core

#endif // AVION_CORE_MATERIAL_H 

