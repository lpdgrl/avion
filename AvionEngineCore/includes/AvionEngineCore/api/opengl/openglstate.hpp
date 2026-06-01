#ifndef AVION_API_BACKEND_OPENGL_STATE_H
#define AVION_API_BACKEND_OPENGL_STATE_H
  
  #include <cstdint>

  #include "AvionEngineCore/api/opengl/opengltypes.hpp"

  namespace avion::api::backend::opengl::detail
  {
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

    struct BlendState
    {
      bool is_enabled{};
      BlendFunc source_factor;
      BlendFunc destination_factor;
    };

    struct ViewportState
    {

    };
  } // namespace avion::api::backend::opengl::detail 

#endif
