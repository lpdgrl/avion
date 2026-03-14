#include "AvionEngineCore/renderer/pipeline.hpp"

namespace avion::gfx {

    Pipeline::Pipeline(core::Scene& scene): scene_(scene) {}

    Pipeline::~Pipeline() {
        delete renderer_;
        std::cout << "Pipeline is destroyed" << '\n';
    }

    void Pipeline::Init(int width, int height) {
        InitShadersStructs();

        renderer_ = new Renderer();

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        renderer_->Init();
        renderer_->SetPerspectiveProjection(45.f, width, height, 0.1f, 50.f);
    }

    void Pipeline::InitShadersStructs() noexcept {
            shader_light_.light_color.name_param = "ligthColor";
            shader_light_.model_matrix.name_param = "model";
            shader_light_.view_pos.name_param = "view_pos";
            shader_light_.view_matrix.name_param = "view";
            
            shader_object_.mat_light_ambient.name_param = "material.ambient";
            shader_object_.mat_light_diffuse.name_param = "material.diffuse";
            shader_object_.mat_light_specular.name_param = "material.specular";
            shader_object_.mat_light_shininess.name_param = "material.shininess";

            shader_object_.screen_aspect.name_param = "scr_aspect";

            shader_object_.model_matrix.name_param = "model";
            shader_object_.view_matrix.name_param = "view";
            shader_object_.view_pos.name_param = "view_pos";
            
            shader_object_.light_ambient.name_param = "light.ambient";
            shader_object_.light_diffuse.name_param = "light.diffuse";
            shader_object_.light_specular.name_param = "light.specular";
            shader_object_.light_position.name_param = "light.position";
    }

    gfx::ShaderObject& Pipeline::GetShaderObjectStruct() noexcept {
        return shader_object_;
    }

    gfx::ShaderLight& Pipeline::GetShaderLigthStruct() noexcept {
        return shader_light_;
    }


    void Pipeline::ChangeCameraPosition(CameraMovement direction, GLfloat delta_time) const noexcept {
        renderer_->ChangeCameraPosition(direction, delta_time);
    }

    void Pipeline::ProcessMouseMovement(double xoffset, double yoffset) const noexcept {
        renderer_->ProcessMouseMovement(xoffset, yoffset);
    }

    void Pipeline::TransferDataToFrameBuffer() noexcept {
        const auto& objects_scene = scene_.GetAllObjects();
        const auto& lights_scene = scene_.GetAllSourceLights();
        
        for (const auto& [type, object] : objects_scene) {
            auto [position, size, color, mixing_color, material] = object.GetParams();
            
            core::Light light;
            for (const auto& [_, curr_light, l_color, l_size] : lights_scene) {  
                shader_light_.light_color.value = l_color.color;
                RenderContext<ShaderLight> render_context{
                    .shader = shader_light_,
                    .position = curr_light.position,
                    .size = l_size.size, 
                    .axis = AxisRotate::NONE,
                    .rotate = 0.f,
                    .key = static_cast<MapKey>(type)
                };
                renderer_->Draw<ShaderLight>(render_context);
                light = curr_light;
            }
            
            shader_object_.light_ambient.value = light.ambient;
            shader_object_.light_diffuse.value = light.diffuse;
            shader_object_.light_specular.value = light.specular;
            shader_object_.light_position.value = light.position;
            
            shader_object_.mat_light_ambient.value = material.ambient;
            shader_object_.mat_light_diffuse.value = material.diffuse;
            shader_object_.mat_light_specular.value = material.specular;
            shader_object_.mat_light_shininess.value = material.shininess;

            RenderContext<ShaderObject> render_context{
                .shader = shader_object_,
                .position = position,
                .size = size,
                .axis = AxisRotate::NONE,
                .rotate = 0.f,
                .key = static_cast<MapKey>(type)
            };

            renderer_->Draw<ShaderObject>(render_context);
        }

    }

} // namespace avion::gfx
