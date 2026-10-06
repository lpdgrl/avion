#ifndef AVION_API_IRENDERER_H
#define AVION_API_IRENDERER_H 1

  #include <string>

  #include "AvionEngineCore/api/backend/renderstate.hpp"
  #include "AvionEngineCore/api/backend/RenderItem.hpp"
  #include "AvionEngineCore/api/backend/RenderItem2D.hpp"

  namespace avion::api::backend
  {
    class IRenderer
    {
      public: 
        using RenderState = backend::detail::RenderState;
        using RenderItem  = backend::detail::RenderItem;
        using RenderItem2D = backend::detail::RenderItem2D;
        using OrthoProjection = backend::detail::OrthoProjection;
        using PerspectiveProjection = backend::detail::PerspectiveProjection;
        using MeshData = backend::detail::MeshData;
        using Texture2dData = backend::detail::Texture2dData;
        using Texture2dId = std::uint32_t;

        IRenderer()  = default;
        ~IRenderer() = default;

        virtual void Init(RenderState state, const std::string& font) = 0;
        virtual void ApplyCurrentState(const RenderState& render_state) noexcept = 0;
        virtual void PrepareDraw() const noexcept = 0;
        virtual std::tuple<std::size_t, std::size_t, std::size_t> Draw(const RenderItem& item) const noexcept = 0;
        virtual void DrawItem2D(const RenderItem2D& item) const noexcept = 0;
        virtual void DrawText(const std::string& text, float x, float y, float scale, glm::vec3 color) noexcept = 0;

        virtual void SetOrthoProjection(const OrthoProjection& projection) noexcept = 0;
        virtual void SetPerspectiveProjection(PerspectiveProjection& projection) noexcept = 0;

        // Interface for interact with graphics objects
        virtual std::uint32_t CreateBuffer(MeshData mesh_data) = 0;
        virtual Texture2dId CreateTexture2D(Texture2dData data) = 0;
        virtual void CreateFrameBuffer(const std::string& name, float width, float height) = 0;

      private:
    };
      
  } // namespace avion::api::backend

#endif