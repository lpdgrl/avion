
#ifndef AVION_CORE_LIGHT_H
#define AVION_CORE_LIGHT_H 

    #include "../../glm/glm.hpp"
    #include "object.hpp"

    namespace avion::core  {
        
        struct Light {
            glm::vec3 position;
        
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };
        
        struct LightParams {
            Light light; 
            Color color;
            Size size;
        };

    } // namespace avion::core 

#endif // AVION_CORE_LIGHT_H
