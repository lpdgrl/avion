#include "AvionEngineCore/api/opengl/types/OpenglBuffer.hpp"

namespace avion::api::backend::opengl
{
  OpenglBuffer::~OpenglBuffer()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
  }

} // namespace avion::api::backend