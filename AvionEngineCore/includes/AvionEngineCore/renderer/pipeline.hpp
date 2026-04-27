#pragma once 

#include "renderer.hpp"
#include "AvionEngineCore/core/scene.hpp"

namespace avion::gfx {

  struct ShaderType_t {
    std::string name;
    ShaderLight type;
  };

  class Pipeline {
  public:
    Pipeline() = delete;
    Pipeline(core::Scene& scene, core::resman::ResourceManager& resman, PipelineQueue& pl_queue);

    Pipeline(const Pipeline& other) = delete;
    Pipeline(Pipeline&& other) = delete;

    Pipeline& operator=(const Pipeline& other) = delete;
    Pipeline& operator=(Pipeline&& other) = delete;

    
    ~Pipeline();

    void Init(int width, int height);
    
    void ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept;
    void ProcessMouseMovement(double xoffset, double yoffset) const noexcept;

    void TransferDataToFrameBuffer() noexcept;

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

