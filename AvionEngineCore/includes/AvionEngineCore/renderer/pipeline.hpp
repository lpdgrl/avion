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
        using Textures    = std::deque<core::Texture>; 
        using TexturePtr  = core::Texture*;
        using ResTextures = std::unordered_map<std::string, TexturePtr>;

        Pipeline() = delete;
        Pipeline(core::Scene& scene);

        Pipeline(const Pipeline& other) = delete;
        Pipeline(Pipeline&& other) = delete;

        Pipeline& operator=(const Pipeline& other) = delete;
        Pipeline& operator=(Pipeline&& other) = delete;

        
        ~Pipeline();

        void Init(int width, int height);
        void InitShadersStructs() noexcept;
        
        void ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept;
        void ProcessMouseMovement(double xoffset, double yoffset) const noexcept;

        ResTextures& GetLoadedResource() noexcept;

        void TransferDataToFrameBuffer() noexcept;

    private:
      ShaderType_t GetShaderTypeByLightType(core::LightType) const noexcept;

      core::Scene& scene_;
      core::resman::ResourceManager m_resman;
      Renderer* renderer_;
      ShaderStorage m_shaders_storage;
      Textures queue_textures_; 
      ResTextures res_; 
    };
    
} // namespace avion::gfx

