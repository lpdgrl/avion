#ifndef AVION_API_IRENDERER_H
#define AVION_API_IRENDERER_H 1

  #include <string>

  #include "AvionEngineCore/api/backend/renderstate.hpp"
  #include "AvionEngineCore/api/backend/RenderItem.hpp"

  namespace avion::api::backend
  {
    class IRenderer
    {
      public: 
        using RenderState = backend::detail::RenderState;
        using RenderItem  = backend::detail::RenderItem;
        using OrthoProjection = backend::detail::OrthoProjection;
        using PerspectiveProjection = backend::detail::PerspectiveProjection;
        using MeshData = backend::detail::MeshData;
        using Texture2dData = backend::detail::Texture2dData;
        using Texture2dId = std::uint32_t;

        IRenderer()  = default;
        ~IRenderer() = default;

        virtual void Init(RenderState state) = 0;
        virtual void ApplyCurrentState(RenderState& render_state) noexcept = 0;
        virtual void PrepareDraw() const noexcept = 0;
        virtual void Draw(const RenderItem& item) const noexcept = 0;

        virtual void SetOrthoProjection(OrthoProjection& projection) noexcept = 0;
        virtual void SetPerspectiveProjection(PerspectiveProjection& projection) noexcept = 0;

        // Interface for interact with graphics objects
        virtual std::uint32_t CreateBuffer(MeshData mesh_data) = 0;
        virtual Texture2dId CreateTexture2D(Texture2dData data) = 0;
        virtual void CreateFrameBuffer(const std::string& name, float width, float height) = 0;

      private:
    };
      
  } // namespace avion::api::backend

#endif