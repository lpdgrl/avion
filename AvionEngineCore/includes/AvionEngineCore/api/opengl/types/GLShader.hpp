#ifndef AVION_API_BACKEND_OPENGL_TYPES_GL_SHADER_H
#define AVION_API_BACKEND_OPENGL_TYPES_GL_SHADER_H 1

  #include <string_view>
  #include <fstream>

  #include "AvionEngineCore/api/opengl/OpenglUtils.hpp"
  #include "AvionEngineCore/macro.h"

  namespace avion::api::backend::opengl
  {
    class GLShader
    {
      public:
        GLShader() = delete;

        GLShader(detail::ShaderType type, std::string_view code);
        GLShader(const GLShader& other) = delete;
        GLShader(GLShader&& other) noexcept = default;

        GLShader& operator=(const GLShader& other) = delete;
        GLShader& operator=(GLShader&& other) noexcept = default;

        auto GetId() const noexcept -> unsigned int;
        auto IsSuccess() const noexcept -> bool;
        
        ~GLShader();

      private:
        friend void swap(GLShader& lhs, GLShader& rhs) noexcept
        {
          using std::swap;

          swap(lhs.m_id, rhs.m_id);
          swap(lhs.m_success_compile, rhs.m_success_compile);
        }

      private:
        unsigned int m_id;
        int m_success_compile{};
    };

  } // namespace avion::api::backend::opengl

#endif