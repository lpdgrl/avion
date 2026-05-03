#include "AvionEngineCore/renderer/framebuffer.hpp"

namespace avion::gfx
{

  FrameBuffer::FrameBuffer(float width, float height)
  : m_width(width)
  , m_height(height)
  {
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);


    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
      AV_LOG_ERROR("FrameBuffer::FrameBuffer: Framebuffer is note complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
  }

  FrameBuffer::FrameBuffer(const FrameBuffer& other)
  : m_fbo(other.m_fbo)
  , m_texture(other.m_texture)
  , m_rbo(other.m_rbo)
  , m_width(other.m_width)
  , m_height(other.m_height)
  {

  }

  FrameBuffer& FrameBuffer::operator=(const FrameBuffer& other)
  {
    if (this == &other)
    {
      return *this;
    }
    m_fbo = other.m_fbo;
    m_rbo = other.m_rbo;
    m_texture = other.m_texture;
    m_width = other.m_width;
    m_height = other.m_height;

    return *this;
  }

  FrameBuffer::~FrameBuffer()
  {
    // glDeleteFramebuffers(1, &m_fbo);
    // glDeleteTextures(1, &m_texture);
    // glDeleteRenderbuffers(1, &m_rbo);
  }

  void FrameBuffer::Bind() const 
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  }

  void FrameBuffer::Unbind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  unsigned int FrameBuffer::GetFrameTextures() const noexcept
  {
    return m_texture;
  }

  void FrameBuffer::RescaleFrameBuffer(float width, float height)
  {
    m_width = width;
    m_height = height;

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
  }

} // namespace avion::gfx