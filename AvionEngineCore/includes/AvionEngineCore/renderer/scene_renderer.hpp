#pragma once 

#include <functional>

#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/api/backend/backend.hpp"

namespace avion::gfx {

  class SceneRenderer {
  public:
    using ModelHandler        = api::backend::Backend::ModelHandler;
    using RenderItem          = api::backend::Backend::RenderItem;
    using Backend             = api::backend::Backend;

    using BackendCallback     = std::function<void(const RenderItem& render_item)>;

    SceneRenderer() = delete;
    SceneRenderer(core::Scene& scene, core::resman::ResourceManager& resman);

    SceneRenderer(const SceneRenderer& other) = delete;
    SceneRenderer(SceneRenderer&& other) = delete;

    SceneRenderer& operator=(const SceneRenderer& other) = delete;
    SceneRenderer& operator=(SceneRenderer&& other) = delete;
    
    ~SceneRenderer();

    void Init(int width, int height);
    void SetBackendCallback(BackendCallback callback);
    void Draw() noexcept;

  private:
    core::Scene& m_scene;
    core::resman::ResourceManager&  m_resman;
    BackendCallback m_cb_backend;
  };
  
} // namespace avion::gfx

