#ifndef AVION_API_RENDER_COMMAND_H
#define AVION_API_RENDER_COMMAND_H

  #include <cstddef>
  #include <cstdint>
  #include <vector>
  
  #include "AvionEngineCore/api/opengl/openglstate.hpp"

  namespace avion::api::backend::detail
  {

    enum class OpenglContext
    { 
      kUnknown        = 0,
      kGlobalState    = 1,
      kDepthTest      = 2,
      kStencilTest    = 3,
      kBlendingState  = 4,
      kFrameBuffer    = 5,
      kVertexBuffer   = 6,
      kTexture2D      = 7,
      kDrawState      = 8,
      kMeshBuffer     = 9,
    };

    enum class OpenglCommand
    {
      kUnknown          = 0,
      kSetClearColor    = 1,
      kSetClear         = 2,
      kSetViewport      = 3,
      kEnable           = 4,
      kDisable          = 5,
      kDepthFunc        = 6,
      kDepthMask        = 7,
      kStencilOp        = 8,
      kStencilFunc      = 9,
      kStencilMask      = 10,
      kBlendFunc        = 11,
      kBlendEquation    = 12,
      kBindFrameBuffer  = 13,
      kBindVertexBuffer = 14,
      kBindTexture2D    = 15,
      kCreateBufferMesh = 16,
      kDrawArrays       = 17,
      kDrawElements     = 18,
    };

    struct MeshBufferData
    {
      using IndexType       = opengl::detail::IndexType;
      using VertexAttribute = opengl::detail::VertexAttribute;

      std::uint32_t mesh_id{};

      std::vector<std::byte> vertex_data;
      std::size_t vertex_stride{};
      std::size_t vertex_count{};
      std::vector<VertexAttribute> vertex_attributes;

      std::vector<std::byte> index_data;
      IndexType index_type = IndexType::kUnsignedInt;
      std::size_t index_count{};
    };

    struct OpenglState
    {
      using BlendEquation = opengl::detail::BlendEquation;
      using BlendFunc     = opengl::detail::BlendFunc;
      using CompareFunc   = opengl::detail::CompareFunc;
      using DrawPrimitive = opengl::detail::DrawPrimitive;
      using IndexType     = opengl::detail::IndexType;
      using StencilAction = opengl::detail::StencilAction;

      // set global state
      bool enable{};
      bool disable{};

      // set clear color
      float red{};
      float green{};
      float blue{};
      float alpha{};

      // set viewport 
      int x_viewport{};
      int y_viewport{};
      std::size_t width_viewport{};
      std::size_t height_viewport{};

      // set clear buffer bit
      bool color_buffer_bit{};
      bool depth_buffer_bit{};
      bool stencil_buffer_bit{};

      // enable depth testing
      bool depth_test{};
      bool depth_mask{};
      CompareFunc depth_func = CompareFunc::fnLess;
      
      // enable stencil testing
      bool stencil_test{};
      CompareFunc stencil_func = CompareFunc::fnAlways;
      StencilAction stencil_fail = StencilAction::fnKeep;
      StencilAction stencil_depth_fail = StencilAction::fnKeep;
      StencilAction stencil_depth_pass = StencilAction::fnKeep;
      std::int32_t stencil_ref{};
      std::uint32_t stencil_mask{};

      // enable blending
      bool blend_mode{};
      BlendFunc blend_source_factor = BlendFunc::fnOne;
      BlendFunc blend_destination_factor = BlendFunc::fnZero;
      BlendEquation blend_equation = BlendEquation::fnAdd;

      // bind objects
      std::uint32_t framebuffer_id{};
      std::uint32_t vertex_buffer_id{};
      std::uint32_t texture_2d_id{};

      // draw call
      DrawPrimitive primitive = DrawPrimitive::kTriangles;
      IndexType index_type = IndexType::kUnsignedInt;
      std::int32_t first_vertex{};
      std::size_t vertex_count{};
      std::size_t index_count{};
      std::size_t index_offset{};
    };

    struct RenderCommand
    {
      OpenglContext               context = OpenglContext::kUnknown;
      std::vector<OpenglCommand>  commands;
      OpenglState                 state{};
      MeshBufferData              mesh_buffer{};
    };

  } // namespace avion::api::backend::detail

#endif
