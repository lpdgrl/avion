#include "AvionEngineCore/api/opengl/types/opengl_mesh.hpp"

namespace avion::api::backend::types
{

  OpenglMesh::OpenglMesh()
  {
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
  }

  OpenglMesh::~OpenglMesh()
  {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
  }

} // namespace avion::api::backend::types