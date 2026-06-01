#ifndef AVION_API_BACKEND_RENDER_STATE_H
#define AVION_API_BACKEND_RENDER_STATE_H

  #include "AvionEngineCore/api/opengl/opengl_renderer.hpp"

  namespace avion::api::backend::detail
  {
    using DepthState   = backend::opengl::detail::DepthState;
    using StencilState = backend::opengl::detail::StencilState;
    using BlendState   = backend::opengl::detail::BlendState; 

    struct RenderState
    {
      // ViewportState viewport_state;
      DepthState    depth_state;
      StencilState  stencil_state;
      BlendState    blend_state;
    };

  } // namespace avion::api::backend

#endif