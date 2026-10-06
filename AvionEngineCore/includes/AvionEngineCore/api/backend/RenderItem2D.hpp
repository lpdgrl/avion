#ifndef AVION_API_BACKEND_RENDER_ITEM2D_H
#define AVION_API_BACKEND_RENDER_ITEM2D_H 1

  #include <span>
  #include <cstdint>

  #include "AvionEngineCore/core/material.hpp"
  #include "AvionEngineCore/core/ModelManager/ModelData.hpp"
  #include "AvionEngineCore/core/TextureManager/TextureHandler.hpp"
  #include "AvionEngineCore/renderer/transform.hpp"
  #include "AvionEngineCore/api/backend/renderstate.hpp"

  #include "../../../../../AvionMath/include/Vector2.hpp"
  #include "../../../../../AvionMath/include/Vector3.hpp"

  namespace avion::api::backend::detail
  {
    struct RenderItem2D
    {
      using ModelHandler = core::modelmanager::detail::ModelHandler;
      using MeshRange = std::span<core::modelmanager::detail::MeshRange>;
      using Transform = gfx::Transform;
      using TextureHandler = core::texturemanager::detail::TextureHandler;
      using MaterialRange = std::span<TextureHandler>;

      // std::uint8_t render_item_option{};

      glm::mat4 view_matrix{};
      av::math::vec::Vector2<float> view_position{};
      av::math::vec::Vector2<float> position;

      Transform     transform;
      ModelHandler  model_handler;
      MeshRange     mesh_range;
      MaterialRange diffuse_range;

      float ratio_light{};
      float ratio_scale{};
      bool is_inside_light_radius{};
    };
  } // namespace avion::api::backend::detail


#endif 