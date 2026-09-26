#include "AvionEngineCore/renderer/scene_renderer.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"


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

    void SceneRenderer::SetWindowCallback(WindowCallback callback)
    {
      m_cb_window = callback;
    }

    void SceneRenderer::PrepareRenderItems() noexcept
    {
      using namespace api::backend::detail;

      auto delta_time = m_cb_window();

      auto&& items = m_scene.GetSceneItems();
      auto camera_data = m_scene.GetCameraData();
      auto& light_src = m_scene.GetCacheLightItems();
      std::vector<LightSrcRenderable> light;

      // Light pass
      if (!light_src.empty())
      {
        LightSrcRenderable light_src_renderable;
        for (const auto* light_item : light_src)
        {
          // auto& light_src_renderable = renderable_item.light_src_renderable;
          // light_src_renderable.shininess = material.shininess;

          light_src_renderable.ambient  = light_item->light->GetAmbient();
          light_src_renderable.diffuse  = light_item->light->GetDiffuse();
          light_src_renderable.specular = light_item->light->GetSpecular();
          light_src_renderable.position = light_item->ptr_model->GetTransform().position;

          switch(light_item->light_type)
          {
            case core::LightType::kDirLight:
            {
              light_src_renderable.light_src_type = LightSrcRenderableType::kDirLightSrc;
              break;
            }
            case core::LightType::kPointLight:
            {
              light_src_renderable.light_src_type = LightSrcRenderableType::kPointLightSrc;
              const auto* light_src = static_cast<core::PointLight*>(light_item->light.get());
              light_src_renderable.constant  = light_src->GetConstant();
              light_src_renderable.linear    = light_src->GetLinear();
              light_src_renderable.quadratic = light_src->GetQuadratic();
              light_src_renderable.number_point_light = m_scene.GetNumberPointLight();
              break;
            }
            case core::LightType::kSpotLight:
            {
              light_src_renderable.light_src_type = LightSrcRenderableType::kSpotLightSrc;
              const auto* light_src = static_cast<core::SpotLight*>(light_item->light.get());
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
          light.emplace_back(light_src_renderable);
        }
      }

      for (const auto& item : items)
      {
        RenderItem renderable_item;
        auto& material = item->ptr_model->GetMaterial();
        
        if(item->ptr_model->HasAnimation())
        {
          renderable_item.has_animation = true;
          item->ptr_model->GetAnimator().UpdateAnimation(delta_time);
          renderable_item.final_bones_matrices = item->ptr_model->GetAnimator().GetFinalBoneMatrices();
        }

        bool is_material = material.type == core::material::MaterialType::kTexture ? true : false;
        if (is_material)
        {
          renderable_item.render_item_option |= g_MaskRenderOption &
            static_cast<std::uint8_t>(RenderOption::kTextureMaterial);

          if (material.material_diffuse_override.has_value())
          {
            auto& mat = material.material_diffuse_override.value();
            renderable_item.material_diffuse_override = mat;
            renderable_item.is_material_override = true;
            if (material.material_specular_override.has_value())
            {
              auto& mat = material.material_specular_override.value();
              renderable_item.material_specular_override = mat;
              renderable_item.is_material_override = true;
            }
          }
          else 
          {
            if (material.diffuse_texture.size() > 0)
            {
              renderable_item.diffuse_range = {material.diffuse_texture.data(), material.diffuse_texture.size()};
            }
            if (material.specular_texture.size() > 0)
            {
              renderable_item.specular_range = {material.specular_texture.data(), material.specular_texture.size()};
            }
            if (material.opacity == core::material::Transparency::kSemiTransparency)
            {
              renderable_item.render_item_option |= g_MaskRenderOption &
                static_cast<std::uint8_t>(RenderOption::kSemiTransparency);
            }
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

        if (!light.empty())
        {
          renderable_item.render_item_option |= g_MaskRenderOption &
            static_cast<std::uint8_t>(RenderOption::kLightRenderable);
          auto& light_src_renderable = light.back();
          light_src_renderable.shininess = material.shininess;
          renderable_item.light_src_renderable = light_src_renderable;
        }
        m_cb_backend(renderable_item);
      }
    }

    void SceneRenderer::PrepareLightItems(std::vector<GpuLightSource>& light_items) noexcept
    {

    }

} // namespace avion::gfx
