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

        bool is_material = item.p_model->GetMaterial().size() > 0 ? true : false;
        if (is_material)
        {
          renderable_item.render_item_option = kMaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kTextureMaterial);
          renderable_item.material_range = {item.p_model->GetMaterial().data(), item.p_model->GetMaterial().size()};
        }
        else
        {
          renderable_item.render_item_option = kMaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kSolidColorMaterial);
          renderable_item.solid_color = item.p_model->GetColor();
        }

        renderable_item.view_matrix = camera_data.view_matrix;
        renderable_item.view_position = camera_data.position;
        renderable_item.model_handler = item.model_handler;
        renderable_item.mesh_range = item.p_model->GetMeshRange();
        renderable_item.transform = item.p_model->GetTransform();
        m_cb_backend(renderable_item);
      }

      auto&& lights = m_scene.GetAllSourceLights();
      for (const auto& light : lights)
      {
        RenderItem renderable_item;
        bool is_material = light.p_model->GetMaterial().size() > 0 ? true : false;
        if (is_material)
        {
          renderable_item.render_item_option = kMaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kTextureMaterial);
          renderable_item.material_range = {light.p_model->GetMaterial().data(), light.p_model->GetMaterial().size()};
        }
        else
        {
          renderable_item.render_item_option = kMaskRenderOption & 
            static_cast<std::uint8_t>(RenderOption::kSolidColorMaterial);
          renderable_item.solid_color = light.p_model->GetColor();
        }

        renderable_item.view_matrix = camera_data.view_matrix;
        renderable_item.view_position = camera_data.position;
        renderable_item.model_handler = light.model_handler;
        renderable_item.mesh_range = light.p_model->GetMeshRange();
        renderable_item.transform = light.p_model->GetTransform();
        m_cb_backend(renderable_item);
      }
    }

} // namespace avion::gfx
