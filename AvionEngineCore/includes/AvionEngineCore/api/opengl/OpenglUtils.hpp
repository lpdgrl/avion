#ifndef AVION_API_BACKEND_OPENGL_UTILS_H
#define AVION_API_BACKEND_OPENGL_UTILS_H 1

#include <glad/glad.h>

namespace avion::api::backend::opengl::detail
{
    enum class ShaderType : int 
    {
      Vertex = 0,
      Fragment = 1,
    };

    auto ShaderTypeToGLShader(ShaderType type) noexcept -> unsigned int;

} // namespace avion::api::backend::opengl::detail

#endif 