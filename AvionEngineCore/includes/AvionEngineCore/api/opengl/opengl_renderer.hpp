#ifndef AVION_API_BACKEND_OPENGL_H
#define AVION_API_BACKEND_OPENGL_H

    #include <memory>

    #include "AvionEngineCore/api/opengl/openglstate.hpp"
    #include "AvionEngineCore/api/opengl/types/opengl_mesh.hpp"
   
    namespace avion::api::backend::opengl
    {
       class OpenglRenderer
       {
        public:
          using DepthState    = detail::DepthState;
          using StencilState  = detail::StencilState;
          using BlendState    = detail::BlendState;
          using OpenglMesh    = types::OpenglMesh;

          OpenglRenderer() = delete;
          OpenglRenderer(DepthState depth_state, StencilState stencil_state, BlendState blend_state);

          OpenglRenderer(const OpenglRenderer&) = delete;
          OpenglRenderer(OpenglRenderer&&)      = delete;

          OpenglRenderer& operator=(const OpenglRenderer&) = delete;
          OpenglRenderer& operator=(OpenglRenderer&&)      = delete;

          ~OpenglRenderer() = default;

          void SubmitDepthState(DepthState depth_state)       noexcept;
          void SubmitStencilState(StencilState stencil_state) noexcept;
          void SubmitBlendState(BlendState blend_state)       noexcept;

          // TODO: изменяет текущий стейт
          void ApplyCurrentState() const noexcept;

          void Draw() const noexcept;

        protected:

        private:
          void ApplyDepthState()    const noexcept;
          void ApplyStencilState()  const noexcept;
          void ApplyBlendState()    const noexcept;

        private:
          DepthState    m_depth_state;
          StencilState  m_stencil_state;
          BlendState    m_blend_state;
          

          // std::vector<std::unique_ptr<OpenglMesh>> m_meshes;
       };
    } // namespace avion::api::backend::opengl

#endif


