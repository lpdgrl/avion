#include "AvionEngineCore/api/opengl/types/GLShader.hpp"

namespace avion::api::backend::opengl
{
  GLShader::GLShader(detail::ShaderType type, std::string_view code)
  {
    const char* char_code = code.data();
    unsigned int gl_type = detail::ShaderTypeToGLShader(type);
    m_id = glCreateShader(gl_type);
    glShaderSource(m_id, 1, &char_code, nullptr);
    glCompileShader(m_id);
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &m_success_compile);    
  }

  auto GLShader::GetId() const noexcept -> unsigned int
  {
    return m_id;
  }

  auto GLShader::IsSuccess() const noexcept -> bool
  {
    char buffer[512]{};
    glGetShaderInfoLog(m_id, 512, nullptr, buffer);
    if (m_success_compile > 0)
    {
      AV_LOG_DEBUG(std::format("GLShader::IsSuccess(): shader with id {} succesfuly compilated {}", m_id, buffer));
      return true;
    }
    AV_LOG_ERROR(std::format("GLShader::IsSuccess(): shader with id {} have error in process compilation it. Error: {}", m_id, buffer));
    return false;
  }
  
  GLShader::~GLShader() 
  {
    if (m_id > 0)
    {
      AV_LOG_DEBUG(std::format("GLShader::~GLShader: shader with id {} is destroyed.", m_id));
      glDeleteShader(m_id);
    }
  }

} // namespace avion::api::backend::opengl
