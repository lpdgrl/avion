
#ifndef AVION_RENDERER_LIGHT_H
#define AVION_RENDERER_LIGHT_H 

    #include "../../glm/glm.hpp"

    namespace avion::gfx::light  {

        struct Light {
            glm::vec3 position;
        
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };

    } // namespace avion::gfx::light 

#endif // AVION_RENDERER_LIGHT_H

