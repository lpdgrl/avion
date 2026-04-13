#pragma once 

#include "renderer.hpp"
#include "AvionEngineCore/core/scene.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

#include <deque>

namespace avion::gfx {

    struct ShaderType_t {
      std::string name;
      ShaderLight type;
    };

    class Pipeline {
    public:
        Pipeline() = delete;
        Pipeline(core::Scene& scene, core::resman::ResourceManager& resman);

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
      void ProcessLight(bool is_texture) noexcept; 
      void ProcessObject() noexcept;

      core::Scene& scene_;
      core::resman::ResourceManager& m_resman;
      Renderer* renderer_;
      ShaderStorage m_shaders_storage;
    };
    
} // namespace avion::gfx

