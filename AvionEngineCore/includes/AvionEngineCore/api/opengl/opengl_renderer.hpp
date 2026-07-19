#ifndef AVION_API_BACKEND_OPENGL_H
#define AVION_API_BACKEND_OPENGL_H

    #include <memory>
    #include <unordered_map>
    
    #include "glad/glad.h"
    #include "glm/glm.hpp"
    #include "glm/gtc/matrix_transform.hpp"

    #include "AvionEngineCore/api/backend/IRenderer.hpp"
    
    // Opengl types
    #include "AvionEngineCore/api/opengl/types/OpenglBuffer.hpp"
    #include "AvionEngineCore/api/opengl/types/OpenglTexture2D.hpp"
    #include "AvionEngineCore/api/opengl/types/OpenglFrameBuffer.hpp"

    // TODO: Moving shader to opengl types 
    #include "AvionEngineCore/renderer/shader.hpp"
   
    namespace avion::api::backend::opengl
    {
      // отрисовывает кадры - draw
      // изменяет состояние - применяет текущий стейт
      // создает mesh-буффер 
      class OpenglRenderer final : public IRenderer
      {
      public:
        using DepthState          = backend::detail::DepthState;
        using StencilState        = backend::detail::StencilState;
        using BlendingState       = backend::detail::BlendingState;
        using ViewportState       = backend::detail::ViewportState;
        using ColorState          = backend::detail::ColorState;
        using RenderState         = backend::detail::RenderState;
        using MeshData            = backend::detail::MeshData;
        using MaterialRange       = backend::detail::RenderItem::MaterialRange;
        using TextureType         = core::texturemanager::detail::TextureType;
        using ShaderStorage       = gfx::ShaderStorage;
        using BufferId            = std::uint32_t;
        using BufferStorage       = std::unordered_map<BufferId, OpenglBuffer>;
        using Texture2dData       = backend::IRenderer::Texture2dData;
        using Texture2dId         = backend::IRenderer::Texture2dId;
        using Texture2dStorage    = std::unordered_map<Texture2dId, OpenglTexture2D>;
        using FrameBufferStorage  = std::unordered_map<std::string, OpenglFrameBuffer>; 

        OpenglRenderer() = delete;
        OpenglRenderer(ShaderStorage& shader_storage);

        OpenglRenderer(const OpenglRenderer&) = delete;
        OpenglRenderer(OpenglRenderer&&)      = delete;

        OpenglRenderer& operator=(const OpenglRenderer&) = delete;
        OpenglRenderer& operator=(OpenglRenderer&&)      = delete;

        ~OpenglRenderer() = default;

        void Init(RenderState state) override;
        void ApplyCurrentState(RenderState& render_state) noexcept override;
        void PrepareDraw() const noexcept override;
        void Draw(const RenderItem& item) const noexcept override; 

        void SetOrthoProjection(OrthoProjection& projection) noexcept override;
        void SetPerspectiveProjection(PerspectiveProjection& projection) noexcept override;

        // GPU Primitives
        BufferId CreateBuffer(MeshData mesh_data) override;
        Texture2dId CreateTexture2D(Texture2dData data) override;
        void CreateFrameBuffer(const std::string& name, float width, float height) override;

        OpenglFrameBuffer& GetFrameBuffer(const std::string name);

      protected:

      private:
        // чтобы простыни не было в apply current state.
        void ApplyDepthState()    const noexcept;
        void ApplyStencilState()  const noexcept;
        void ApplyBlendingState() const noexcept;
        void ApplyViewportState() const noexcept;
        void ApplyColorState()    const noexcept;

        void SubmitProjectionMatrixToShader() noexcept;
        void BindTexture2D(const MaterialRange& range, int& number) const noexcept;
        void UnBindTexture2D(const MaterialRange& range) const noexcept;

      private:
        DepthState    m_depth_state;
        StencilState  m_stencil_state;
        BlendingState m_blend_state;
        ViewportState m_viewport_state;
        ColorState    m_color_state;

        ShaderStorage& m_shader_storage;

        BufferStorage m_buffer_storage;
        BufferId m_last_buffer_id = 0;

        Texture2dId m_last_texture_id = 0;
        Texture2dStorage m_texture2d_storage;

        FrameBufferStorage m_framebuffer_storage;

        glm::mat4 m_projection;
      };
    } // namespace avion::api::backend::opengl

    namespace avion::api::backend::opengl::detail
    {
      GLenum ToOpenglCompareFunc(backend::detail::CompareFunc func);
      GLenum ToOpenglStencilAction(backend::detail::StencilAction action);
    } // namespace avion::backend::opengl::detail

#endif


