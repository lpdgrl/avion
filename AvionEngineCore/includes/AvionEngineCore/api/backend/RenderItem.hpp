#ifndef AVION_API_BACKEND_RENDER_ITEM_H
#define AVION_API_BACKEND_RENDER_ITEM_H 1

  #include <span>
  #include <cstdint>

  #include "AvionEngineCore/core/material.hpp"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"
  #include "AvionEngineCore/renderer/transform.hpp"
  #include "AvionEngineCore/api/backend/renderstate.hpp"

  namespace avion::api::backend::detail
  {
    static constexpr std::uint8_t g_MaskRenderOption = static_cast<std::uint8_t>(0xFFFFFFFF);
    
    enum class RenderOption : std::uint8_t 
    {
      kSolidColorMaterial = 128,
      kTextureMaterial    = 64,
      kLightRenderable    = 32,
      kSemiTransparency   = 16,
    };

    enum class LightSrcRenderableType : std::uint8_t
    {
      kUnSpecified   = 0,
      kDirLightSrc   = 1,
      kPointLightSrc = 2,
      kSpotLightSrc  = 3,
    };

    inline bool operator==(std::uint8_t lhs, RenderOption rhs) { return (lhs == static_cast<std::uint8_t>(rhs)); }
    inline bool operator==(RenderOption lhs, std::uint8_t rhs) { return (static_cast<std::uint8_t>(lhs) == rhs); }

    struct LightSrcRenderable
    {
      LightSrcRenderableType light_src_type = LightSrcRenderableType::kUnSpecified;

      glm::vec3 ambient{};
      glm::vec3 diffuse{};
      glm::vec3 specular{};
      
      // for spot light
      glm::vec3 position{};
      glm::vec3 direction{};

      float shininess{};
      float constant{};
      float linear{};
      
      float quadratic{};
      float cutoff{};
      float outer_cutoff{};

      std::int32_t number_point_light{};
      std::int32_t number_spot_light{};
    };

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

      Transform     transform;
      ModelHandler  model_handler;
      MeshRange     mesh_range;
      MaterialRange diffuse_range;
      MaterialRange specular_range;

      LightSrcRenderable light_src_renderable;
      RenderState render_state;
    };
  } // namespace avion::api::backend::detail


#endif 