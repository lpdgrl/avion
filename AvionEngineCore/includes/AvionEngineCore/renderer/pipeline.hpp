#pragma once 

#include "renderer.hpp"
#include "AvionEngineCore/core/scene.hpp"

// Forward declaration
namespace avion::editor::detail
{
  struct SelectionContext;
}

// Forward declaration
namespace avion::core
{
  struct CameraState;
}

namespace avion::gfx {

  struct ShaderType_t {
    std::string name;
    ShaderLight type;
  };

  class Pipeline {
  public:
    using RenderState         = core::RenderState;
    using SceneLight          = core::SceneLight;
    using SceneObject         = core::SceneObject;
    using ModelHandler        = core::ModelHandler;
    using SelectionContext    = editor::detail::SelectionContext;

    Pipeline() = delete;
    Pipeline(core::Scene& scene, core::resman::ResourceManager& resman, PipelineQueue& pl_queue, RenderState& render_state);

    Pipeline(const Pipeline& other) = delete;
    Pipeline(Pipeline&& other) = delete;

    Pipeline& operator=(const Pipeline& other) = delete;
    Pipeline& operator=(Pipeline&& other) = delete;
    
    ~Pipeline();

    void Init(int width, int height);
    
    void ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept;
    void ProcessMouseMovement(double xoffset, double yoffset) const noexcept;

    void DrawObjects() noexcept;
    void DrawSelectableObjects(SelectionContext& selection_ctx) noexcept;

    glm::vec3 GetCameraPosition() const noexcept;

  private:
    void ProcessLights(const std::string& name_shader) noexcept; 
    void ProcessModels() noexcept;
    void ProcessObjects() noexcept;

    void ProcessMesh() noexcept;

    core::Scene&                    scene_;
    core::resman::ResourceManager&  m_resman;
    PipelineQueue&                  m_pl_queue;
    Renderer*                       renderer_;
    ShaderStorage                   m_shaders_storage;
  };
  
} // namespace avion::gfx

