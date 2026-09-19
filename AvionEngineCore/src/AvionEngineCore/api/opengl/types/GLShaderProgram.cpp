#include "AvionEngineCore/api/opengl/types/GLShaderProgram.hpp"


namespace avion::api::backend::opengl
{
  GLShaderProgram::GLShaderProgram()
  : m_id(glCreateProgram())
  {
    // AV_LOG_DEBUG(std::format("GLShaderProgram::GLShaderProgram(): Created ShaderProgram with id {}", m_id))
  }

  auto GLShaderProgram::GetId() const noexcept -> unsigned int
  {
    return m_id;
  }

  auto GLShaderProgram::AttachShader(const GLShader& shader) const -> void
  {
    auto id_shader = shader.GetId();
    glAttachShader(m_id, id_shader);
  }

  auto GLShaderProgram::LinkProgram() -> void
  {
    glLinkProgram(m_id);
    glGetProgramiv(m_id, GL_LINK_STATUS, &m_success);
  }

  auto GLShaderProgram::IsSuccess() const -> bool
  {
    char buffer[512]{};
    glGetShaderInfoLog(m_id, 512, nullptr, buffer);
    if (m_success > 0)
    {
      AV_LOG_DEBUG(std::format("GLShaderProgram::IsSuccess() program with id {} succesfuly linked {}", m_id, buffer));
      return true;
    }
    AV_LOG_ERROR(std::format("GLShaderProgram::IsSuccess() : shader with id {} have error in it process linked. Error: {}", m_id, buffer));
    return false;
  }

  GLShaderProgram::~GLShaderProgram()
  {
    if (m_id > 0)
    {
      AV_LOG_DEBUG(std::format("GLShaderProgram::~GLShaderProgram: shader program with id {} is destroyed.", m_id));
      glDeleteProgram(m_id);
    }
  }
  
} // namespace avion::api::backend::opengl
