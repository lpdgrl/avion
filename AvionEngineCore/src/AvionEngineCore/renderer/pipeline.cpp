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

            shader_object_.light_ambient.name_param = "kAmbient";
            shader_object_.light_specular.name_param = "kSpecular";
            shader_object_.light_shininess.name_param = "kShininess";
            shader_object_.screen_aspect.name_param = "scr_aspect";
            shader_object_.model_matrix.name_param = "model";
            shader_object_.view_matrix.name_param = "view";
            shader_object_.view_pos.name_param = "view_pos";
            shader_object_.object_color.name_param = "objectColor";
            shader_object_.light_color.name_param = "ligthColor";
            shader_object_.light_pos.name_param = "ligthPos";
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
        
        core::Object* light = scene_.GetObject(core::TypeObject::kLight);
        glm::vec3 light_pos;
        glm::vec3 light_color;

        if (light != nullptr) {
            light_pos = static_cast<glm::vec3>(light->GetPosition());
            light_color = static_cast<glm::vec3>(light->GetColor());
        }

        for (const auto& [type, object] : objects_scene) {
            auto [position, size, color, mixing_color] = object.GetParams();
            shader_object_.object_color.value = mixing_color;
            
            if (type == core::TypeObject::kLight) {
                shader_light_.light_color.value = color;
                RenderContext<ShaderLight> render_context{
                    .shader = shader_light_,
                    .position = position,
                    .size = size,
                    .axis = AxisRotate::NONE,
                    .rotate = 0.f,
                    .key = static_cast<MapKey>(type)
                };
                renderer_->Draw<ShaderLight>(render_context);
                continue;
            }

            shader_object_.light_pos.value = light_pos;
            shader_object_.light_color.value = light_color;

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
