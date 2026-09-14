#ifndef AVION_API_BACKEND_RENDER_STATE_H
#define AVION_API_BACKEND_RENDER_STATE_H

  #include <cstddef>
  #include <cstdint>
  #include <span>

  namespace avion::api::backend::detail
  {
    struct OrthoProjection
    {
      float left{};
      float width{};
      float bottom{};
      float height{};
      float z_near{};
      float z_far{};
    };

    struct PerspectiveProjection
    {
      float fov{};
      unsigned int width{};
      unsigned int height{};
      float near{};
      float far{};
    };

    enum class CompareFunc
    {
      Never     = 0,
      Less      = 1,
      Equal     = 2,
      Lequal    = 3,
      Greater   = 4,
      Notequal  = 5,
      Gequal    = 6,
      Always    = 7,
    };

    enum class StencilAction
    {
      Zero          = 0,
      Keep          = 1,
      Replace       = 2,
      Increment     = 3,
      IncrementWrap = 4,
      Decrement     = 5,
      DecrementWrap = 6,
      Invert        = 7,
    };

    enum class BlendingFunc
    {
      Zero                      = 0,
      One                       = 1,
      SourceColor               = 2,
      OneMinusColor             = 3,
      OneMinusSourceColor       = 4,
      DestinationColor          = 5,
      OneMinusDestinationColor  = 6,
      SourceAlpha               = 7,
      OneMinusSourceAlpha       = 8,
      DestinationAlpha          = 9,
      OneMinusDestinationAlpha  = 10,
      ConstantColor             = 11,
      OneMinusConstantColor     = 12,
      ConstantAlpha             = 13,
      OneMinusConstantAlpha     = 14,
    };

    enum class BlendEquation
    {
      Add             = 0,
      Subtract        = 1,
      ReverseSubtract = 2,
      Min             = 3,
      Max             = 4,
    };

    struct DepthState
    {
      bool enabled    = false;
      bool depth_mask = false;

      CompareFunc depth_func = CompareFunc::Less;

      bool operator==(const DepthState& rhs) const noexcept
      {
        return (enabled == rhs.enabled)
          && (depth_mask == rhs.depth_mask)
          && (depth_func == rhs.depth_func);
      }
    };

    struct StencilState
    {
      bool enabled = false;

      CompareFunc stencil_func = CompareFunc::Always;

      StencilAction stencil_fail       = StencilAction::Keep;
      StencilAction stencil_depth_fail = StencilAction::Keep;
      StencilAction stencil_depth_pass = StencilAction::Keep;

      std::int32_t stencil_ref        = 0x00;
      std::uint32_t stencil_func_mask = 0x00;
      std::uint32_t stencil_mask      = 0x00;

      bool operator==(const StencilState& rhs) const noexcept
      {
        return (enabled == rhs.enabled)
          && (stencil_func == rhs.stencil_func)
          && (stencil_fail == rhs.stencil_fail)
          && (stencil_depth_fail == rhs.stencil_depth_fail)
          && (stencil_depth_pass == rhs.stencil_depth_pass)
          && (stencil_ref == rhs.stencil_ref)
          && (stencil_func_mask == rhs.stencil_func_mask)
          && (stencil_mask == rhs.stencil_mask);
      }
    };

    struct BlendingState
    {
      bool enabled = false;

      BlendingFunc blend_source_factor      = BlendingFunc::One;
      BlendingFunc blend_destination_factor = BlendingFunc::Zero;

      BlendEquation blend_equation = BlendEquation::Add;

      bool operator==(const BlendingState& rhs) const noexcept
      {
        return (enabled == rhs.enabled)
          && (blend_source_factor == rhs.blend_source_factor)
          && (blend_destination_factor == rhs.blend_destination_factor)
          && (blend_equation == rhs.blend_equation);
      }
    };

    struct ColorState
    {
      float red{};
      float green{};
      float blue{};
      float alpha{};

      bool operator==(const ColorState& rhs) const noexcept
      {
        // TODO: сравнивать чистые флоаты такое себе
        return ((red == rhs.red)
        && (green == rhs.green)
        && (blue == rhs.blue)
        && (alpha == rhs.alpha));
      }
    };

    struct ViewportState
    {
      int x_viewport{};
      int y_viewport{};
      std::size_t width_viewport{};
      std::size_t height_viewport{};

      bool operator==(const ViewportState& rhs) const noexcept
      {
        return (x_viewport == rhs.x_viewport)
          && (y_viewport == rhs.y_viewport)
          && (width_viewport == rhs.width_viewport)
          && (height_viewport == rhs.height_viewport);
      }
    };

    struct RenderState
    {
      std::string   name_framebuffer;
      bool enabled_framebuffer = false;

      DepthState    depth_state;
      StencilState  stencil_state;
      BlendingState blend_state;
      ViewportState viewport_state;
      ColorState    color_state;

      bool operator==(const RenderState& rhs) const noexcept
      {
        return (depth_state == rhs.depth_state)
          && (stencil_state == rhs.stencil_state)
          && (blend_state == rhs.blend_state)
          && (viewport_state == rhs.viewport_state)
          && (color_state == rhs.color_state);
      }
    };

    struct MeshData
    {
      std::span<const std::byte> vertices;
      std::uint64_t vertices_size{};

      std::span<const unsigned int> indices;
      std::uint64_t indices_size{};

      std::uint64_t stride_size{};

      std::uint64_t offset_position{};
      std::uint64_t offset_normals{};
      std::uint64_t offset_tex_coords{};
      std::uint64_t offset_bone_ids{};
      std::uint64_t offset_weights{};
    };

    struct Texture2dData
    {
      std::uint32_t color_channels = 0;
      std::uint32_t width = 0;
      std::uint32_t height = 0;
      std::span<std::byte> buffer;
    };
  } // namespace avion::api::backend

#endif
