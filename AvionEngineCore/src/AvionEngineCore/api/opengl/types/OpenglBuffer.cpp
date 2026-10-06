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

  auto OpenglBuffer::SetupTextBuffer() -> void
  {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindVertexArray(0);
  }

} // namespace avion::api::backend
