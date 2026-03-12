#pragma once 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include "AvionEngineCore/core/object.hpp"

#include <vector>

// Forward Declaration
namespace avion::core {
    struct SceneObject;
    enum class TypeObject; 
} // namespace avion::core

// Forward Declaration
namespace avion::gfx::material {
   struct Material; 
} // namespace avion::gfx::material

namespace avion::gui {
    struct Logs {
        int    fps   = 0;
        double delay = 0;
        double x_px  = 0;
        double y_px  = 0;
        double x_ndc = 0;
        double y_ndc = 0;
    };

    struct WidgetObjectParams {
        core::TypeObject type_obj;
        core::TypeMaterial type_mat;
        core::ObjectParams params;
        bool state_button_addobj;
    };

    class Widget {
    public:
        
        Widget() = delete;
        Widget(GLFWwindow* window);

        Widget(const Widget& other) = delete;
        Widget(Widget&& other) = delete;

        ~Widget();

        void Init();
        void Frame();
        void Render();
        void CleanUp();

        //void WindowAddObject(core::TypeObject& type, glm::vec3& position, glm::vec3& size, glm::vec3& color, bool& state_button) const;
        void WindowAddObject(WidgetObjectParams& object_params) const;
        void WindowLigthColor(glm::vec3& position, glm::vec3& color) const;
        int WindowListObjects(const std::vector<core::SceneObject>& objects) const;
        void WindowLogs(Logs logs) const;
        void WidgetShader(float& kAmbient, float& kSpecular, float& kShininess) const;
    private:
        GLFWwindow* window_ = nullptr;
        ImGuiIO& io_;
};

} // namespace avion::widget
