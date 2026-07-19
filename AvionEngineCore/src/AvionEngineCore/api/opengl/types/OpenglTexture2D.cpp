#include "AvionEngineCore/api/opengl/types/OpenglTexture2D.hpp"


namespace avion::api::backend::opengl
{
  OpenglTexture2D::~OpenglTexture2D()
  {
    glDeleteTextures(1, &m_texture_id);
  }

  void OpenglTexture2D::Bind() const noexcept
  {
    // if (m_texture_id == 1)
    // {
    //   glActiveTexture(GL_TEXTURE0);
    // }
    // else 
    // {
    //   glActiveTexture(GL_TEXTURE0 + static_cast<int>(m_texture_id));
    // 
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
  }

  void OpenglTexture2D::UnBind() const noexcept
  {
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} // namespace avion::api::backend::opengl п