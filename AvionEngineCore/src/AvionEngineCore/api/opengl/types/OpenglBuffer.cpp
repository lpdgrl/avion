#include "AvionEngineCore/api/opengl/types/OpenglBuffer.hpp"

namespace avion::api::backend::opengl
{

  OpenglBuffer::OpenglBuffer(OpenglBuffer&& rhs) noexcept
  : m_vao(std::exchange(rhs.m_vao, 0))
  , m_vbo(std::exchange(rhs.m_vbo, 0))
  , m_ebo(std::exchange(rhs.m_ebo, 0))
  , m_indices_size(std::exchange(rhs.m_indices_size, 0))
  {
  }

  OpenglBuffer& OpenglBuffer::operator=(OpenglBuffer&& rhs) noexcept
  {
    if (this != &rhs)
    {
      m_vao = std::exchange(rhs.m_vao, 0);
      m_vbo = std::exchange(rhs.m_vbo, 0);
      m_ebo = std::exchange(rhs.m_ebo, 0);
      m_indices_size = std::exchange(rhs.m_indices_size, 0);
    }
    return *this;
  }

  OpenglBuffer::~OpenglBuffer()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
  }

} // namespace avion::api::backend
