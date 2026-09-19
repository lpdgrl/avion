#include "AvionEngineCore/api/opengl/OpenglUtils.hpp"

namespace avion::api::backend::opengl::detail
{
  auto ShaderTypeToGLShader(ShaderType type) noexcept -> unsigned int
  {
    switch(type)
    {
      case ShaderType::Vertex:
      {
        return GL_VERTEX_SHADER; 
      }
      case ShaderType::Fragment:
      {
        return GL_FRAGMENT_SHADER;
      }
      // TODO: Add default value for enum TypeShader
      default: 
      {
        return GL_ZERO;
      }
    }
  }
} // namespace avion::api::backend::opengl::detail
