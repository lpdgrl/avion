#pragma once 

#include "renderer.hpp"
#include "../core/scene.hpp"

#include <deque>

namespace avion::gfx {

    class Pipeline {
    public:
        using Textures    = std::deque<core::Texture>; 
        using TexturePtr  = core::Texture*;
        using ResTextures = std::unordered_map<std::string,TexturePtr>;

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

        gfx::ShaderObject& GetShaderObjectStruct() noexcept;
        gfx::ShaderLight& GetShaderLigthStruct() noexcept;
        ResTextures& GetLoadedResource() noexcept;

        void TransferDataToFrameBuffer() noexcept;

    private:
        core::Scene& scene_;
        gfx::Renderer* renderer_;
        gfx::ShaderObject shader_object_;
        gfx::ShaderLight shader_light_;

        Textures queue_textures_; 
        ResTextures res_; 
    };
    
} // namespace avion::gfx

