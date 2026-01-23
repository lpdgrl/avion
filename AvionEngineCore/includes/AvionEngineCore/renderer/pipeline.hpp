#pragma once 

#include "renderer.hpp"
#include "../core/scene.hpp"

namespace avion::gfx {

    class Pipeline {
    public:
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

        void TransferDataToFrameBuffer() noexcept;

    private:
        core::Scene& scene_;
        gfx::Renderer* renderer_;
        gfx::ShaderObject shader_object_;
        gfx::ShaderLight shader_light_;
    };
    
} // namespace avion::gfx

