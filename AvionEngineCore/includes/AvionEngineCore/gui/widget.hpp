#pragma once 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include "../core/light.hpp"

#include <vector>
#include <optional>
#include <unordered_map>

// Forward Declaration from core
namespace avion::core {
    struct SceneObject;
    struct SceneLight;
    
    enum class TypeObject; 
} // namespace avion::core

namespace avion::gfx {
    using TexturePtr = core::Texture*;
    using ResTextures = std::unordered_map<std::string, TexturePtr>; 
};

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
        core::PrefabMaterial type_mat;
        core::ObjectParams params;
        bool state_button_addobj;
    };

    class Widget {
    public:
        
        Widget() = delete;
        Widget(GLFWwindow* window, gfx::ResTextures& res);

        Widget(const Widget& other) = delete;
        Widget(Widget&& other) = delete;

        ~Widget();

        void Init();
        void Frame();
        void Render();
        void CleanUp();

        std::optional<WidgetObjectParams> WindowAddObject() const;
        int WindowListObjects(const std::vector<core::SceneObject>& objects) const;
        
        void WindowLogs(Logs logs) const;
        bool WindowMaterial(core::ObjectParams&) const;
        
        bool w_LightProperties(core::LightParams&) const;
        int w_ListLights(const std::vector<core::SceneLight>& lights) const;
    private:
        GLFWwindow* window_ = nullptr;
        ImGuiIO& io_;
        gfx::ResTextures& res_;
};

} // namespace avion::widget
