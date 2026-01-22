#pragma once

#include "glad/glad.h"
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace avion::gfx {

    class Shader {
    public:
        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);

        ~Shader();
        
        // use/activate the shader
        void use();

        // utility uniform functions 
        void setBool (const std::string &name, bool value) const;
        void setInt (const std::string &name, int value) const;
        void setFloat (const std::string &name, float value) const;
        void setMat4 (const std::string &name, glm::mat4& value) const;
        void setVec3 (const std::string &name, glm::vec3& value) const;

        unsigned int GetID() const noexcept;
        unsigned int GetID() noexcept;
    
    private:
        // the program ID
        unsigned int ID;

    };

} // namespace avion::gfx