#ifndef AVION_API_BACKEND_RENDER_ITEM_H
#define AVION_API_BACKEND_RENDER_ITEM_H 1

  #include <span>
  #include <cstdint>

  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"
  #include "AvionEngineCore/renderer/transform.hpp"

  namespace avion::api::backend::detail
  {
    static constexpr std::uint8_t kMaskRenderOption = static_cast<std::uint8_t>(0xFFFFFFFF);
    
    enum class RenderOption : std::uint8_t 
    {
      kSolidColorMaterial = 128,
      kTextureMaterial    = 64,
    };

    inline bool operator==(std::uint8_t lhs, RenderOption rhs) { return (lhs == static_cast<std::uint8_t>(rhs)); }
    inline bool operator==(RenderOption lhs, std::uint8_t rhs) { return (static_cast<std::uint8_t>(lhs) == rhs); }

    struct RenderItem
    {
      using ModelHandler = core::modelmanager::detail::ModelHandler;
      using MeshRange = std::span<core::modelmanager::detail::MeshRange>;
      using Transform = gfx::Transform;
      using TextureHandler = core::texturemanager::detail::TextureHandler;
      using MaterialRange = std::span<TextureHandler>;

      std::uint8_t render_item_option{};

      glm::mat4 view_matrix{};
      glm::vec3 view_position{};
      glm::vec3 solid_color{};

      ModelHandler model_handler;
      MeshRange mesh_range;
      Transform transform;
      MaterialRange material_range;
    };
  } // namespace avion::api::backend::detail


#endif 