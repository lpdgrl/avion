#include "AvionEngineCore/api/opengl/opengl_renderer.hpp"

namespace avion::api::backend::opengl
{
  OpenglRenderer::OpenglRenderer(DepthState depth_state, StencilState stencil_state, BlendState blend_state)
  : m_depth_state(depth_state)
  , m_stencil_state(stencil_state)
  , m_blend_state(blend_state)
  {
    ApplyCurrentState();
  }

  void OpenglRenderer::SubmitDepthState(DepthState depth_state) noexcept
  {
    
  }

  void OpenglRenderer::SubmitStencilState(StencilState stencil_state) noexcept
  {

  }

  void OpenglRenderer::SubmitBlendState(BlendState blend_state) noexcept
  {

  }

  // Change current opengl state
  void OpenglRenderer::ApplyCurrentState() const noexcept
  {
    ApplyDepthState();
    ApplyStencilState();
    ApplyBlendState();
  } 

  void OpenglRenderer::Draw() const noexcept
  {

  }

  void OpenglRenderer::ApplyDepthState() const noexcept
  {
    if (m_depth_state.is_enabled)
    {
      glEnable(GL_DEPTH_TEST);
      glDepthMask(m_depth_state.is_enable_write);
      glDepthFunc(static_cast<GLenum>(m_depth_state.fn_cmp));
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }
  }

  void OpenglRenderer::ApplyStencilState() const noexcept
  {

  }

  void OpenglRenderer::ApplyBlendState() const noexcept
  {

  }
} // namespace avion::api::backend::opengl