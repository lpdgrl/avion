#ifndef AVION_API_BACKEND_OPENGL_TYPES_GL_SHADER_PROGRAM_H
#define AVION_API_BACKEND_OPENGL_TYPES_GL_SHADER_PROGRAM_H 1

#include "AvionEngineCore/api/opengl/types/GLShader.hpp"

namespace avion::api::backend::opengl
{
  class GLShaderProgram
  {
    public:
      GLShaderProgram();

      GLShaderProgram(const GLShaderProgram& other) = delete;
      GLShaderProgram(GLShaderProgram&& other) noexcept = default;
      
      GLShaderProgram& operator=(const GLShaderProgram& other) = delete;
      GLShaderProgram& operator=(GLShaderProgram&& other) noexcept = default;

      auto GetId() const noexcept -> unsigned int;
      auto AttachShader(const GLShader& shader) const -> void;
      auto IsSuccess() const -> bool;

      auto LinkProgram() -> void;

      ~GLShaderProgram();

    private:
      friend void swap(GLShaderProgram& lhs, GLShaderProgram& rhs) noexcept
      {
        using std::swap;
        swap(lhs.m_id, rhs.m_id);
        swap(lhs.m_success, rhs.m_success);
      }

    protected:

    private:
      unsigned int m_id;
      int m_success;
  };
} // namespace avion::api::backend::opengl



#endif