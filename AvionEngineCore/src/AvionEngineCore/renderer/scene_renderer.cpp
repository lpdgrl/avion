#include "AvionEngineCore/renderer/scene_renderer.hpp"
#include "AvionEngineCore/renderer/pipeline_queue.hpp"

#include "AvionEngineCore/core/resource_manager.hpp"

// TODO: THIS IS WRONG!!! VERY WRONGG. THE ABSTRACTION LEAKING FROM EDITOR CONTEXT
#include "../../../../AvionEngineEditor/includes/AvionEngineEditor/editor/editor_context.hpp"

namespace avion::gfx {

    SceneRenderer::SceneRenderer(core::Scene& scene, core::resman::ResourceManager& resman)
    : m_scene(scene)
    , m_resman(resman) 
    {
      
    }

    SceneRenderer::~SceneRenderer() {
        AV_LOG_INFO("SceneRenderer::~SceneRenderer: SceneRenderer is destroyed");
    }

    void SceneRenderer::Init(int width, int height) {
      AV_LOG_TODO("SceneRenderer::Init(int width, int height): TO DO NOTHING")
    }

    void SceneRenderer::SetBackendCallback(BackendCallback callback)
    {
      m_cb_backend = callback;
    }
    
    void SceneRenderer::PrepareRenderItems() noexcept 
    {
      using namespace api::backend::detail;

      auto&& items = m_scene.GetSceneItems();
      auto camera_data = m_scene.GetCameraData();
      for (const auto& item : items)
      {
        RenderItem renderable_item;
        auto& light_src_renderable = renderable_item.light_src_renderable;
        auto& material = item->ptr_model->GetMaterial();
        if (item->item_type == core::ItemType::kSourceLight)
        {
          renderable_item.render_item_option |= g_MaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kLightRenderable);

          const auto& light_item = static_cast<const core::LightItem&>(*item.get());
          light_src_renderable.ambient  = light_item.light->GetAmbient();
          light_src_renderable.diffuse  = light_item.light->GetDiffuse();
          light_src_renderable.specular = light_item.light->GetSpecular();

          switch(light_item.light_type)
          {
            case core::LightType::kDirLight:
            {
              light_src_renderable.light_src_type = LightSrcRenderableType::kDirLightSrc;
              break;
            }
            case core::LightType::kPointLight:
            {
              light_src_renderable.light_src_type = LightSrcRenderableType::kPointLightSrc;
              auto* light_src = static_cast<core::PointLight*>(light_item.light.get());
              light_src_renderable.constant  = light_src->GetConstant();
              light_src_renderable.linear    = light_src->GetLinear();
              light_src_renderable.quadratic = light_src->GetQuadratic();
              light_src_renderable.number_point_light = m_scene.GetNumberPointLight();
              break;
            }
            case core::LightType::kSpotLight:
            {
              light_src_renderable.light_src_type = LightSrcRenderableType::kSpotLightSrc;
              auto* light_src = static_cast<core::SpotLight*>(light_item.light.get());
              light_src_renderable.constant     = light_src->GetConstant();
              light_src_renderable.linear       = light_src->GetLinear();
              light_src_renderable.quadratic    = light_src->GetQuadratic();
              light_src_renderable.cutoff       = light_src->GetCutOff();
              light_src_renderable.outer_cutoff = light_src->GetOuterCutOff();
              light_src_renderable.number_spot_light = m_scene.GetNumberSpotLight();
              light_src_renderable.direction    = light_src->GetDirection();
              break;
            }
          }
        }
 
        bool is_material = material.type == core::material::MaterialType::kTexture ? true : false;
        if (is_material)
        {
          renderable_item.render_item_option |= g_MaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kTextureMaterial);

          if (material.diffuse_texture.size() > 0)
          {
            renderable_item.diffuse_range = {material.diffuse_texture.data(), material.diffuse_texture.size()};
          }
          if (material.specular_texture.size() > 0)
          {
            renderable_item.specular_range = {material.specular_texture.data(), material.specular_texture.size()};
          }
        }
        else
        {
          renderable_item.render_item_option |= g_MaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kSolidColorMaterial);
          renderable_item.solid_color = material.color;
        }

        renderable_item.view_matrix = camera_data.view_matrix;
        renderable_item.view_position = camera_data.position;
        renderable_item.model_handler = item->model_handler;
        renderable_item.mesh_range = item->ptr_model->GetMeshRange();
        renderable_item.transform = item->ptr_model->GetTransform();
        renderable_item.light_src_renderable.shininess = material.shininess;

        m_cb_backend(renderable_item);
      }
    }

} // namespace avion::gfx
