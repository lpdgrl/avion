#ifndef AVION_API_BACKEND_RENDER_STATE_H
#define AVION_API_BACKEND_RENDER_STATE_H

  #include "glad/glad.h"

  #include <cstdint>

  namespace avion::api::backend::detail
  {

    enum class CompareFunc 
    {
      fnNever         = GL_NEVER,
      fnLess          = GL_LESS,
      fnLessEqual     = GL_LEQUAL,
      fnGreater       = GL_GREATER,
      fnGreaterEqual  = GL_GEQUAL,
      fnEqual         = GL_EQUAL,
      fnNotEqual      = GL_NOTEQUAL,
      fnAlways        = GL_ALWAYS,
    };

    enum class StencilAction
    {
      fnKeep          = GL_KEEP,
      fnZero          = GL_ZERO,
      fnReplace       = GL_REPLACE,
      fnIncrement     = GL_INCR,
      fnIncrementWrap = GL_INCR_WRAP,
      fnDecrement     = GL_DECR,
      fnDecrementWrap = GL_DECR_WRAP,
      fnInvert        = GL_INVERT,
    };

    struct DepthState
    {
      CompareFunc fn_cmp = CompareFunc::fnAlways;
      bool        is_enabled{};
      bool        is_enable_write{};
    };

    struct StencilState
    {
      CompareFunc   fn_cmp      = CompareFunc::fnAlways;
      StencilAction fn_action   = StencilAction::fnKeep;
      std::uint32_t mask{};
      bool          is_enabled{};
    };

    struct ViewportState
    {

    };

    struct BlendState
    {
      bool is_enabled{};
    };

    struct RenderState
    {
      // ViewportState viewport_state;
      DepthState    depth_state;
      StencilState  stencil_state;
      BlendState    blend_state;
    };
  } // namespace avion::api::backend

#endif