#pragma once 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include "../core/light.hpp"

#include <vector>
#include <optional>
#include <unordered_map>
#include <ranges>

// Forward Declaration from core
namespace avion::core {
    struct SceneObject;
    struct SceneLight;
    
    enum class ObjectType;
    enum class LightType;
} // namespace avion::core

namespace avion::core::resman
{
  class ResourceManager;
}

namespace avion::ui::widget {
    struct Logs {
        int    fps   = 0;
        double delay = 0;
        double x_px  = 0;
        double y_px  = 0;
        double x_ndc = 0;
        double y_ndc = 0;
    };

    struct WidgetObjectParams {
      core::ObjectType type_obj;
      core::PrefabMaterial type_mat;
      core::ObjectParams params;
    };

    class Widget {
    public:
        Widget() = delete;
        Widget(GLFWwindow* window, core::resman::ResourceManager& m_resman);

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
       
        std::optional<core::LightType> w_LightAdd() const;
        bool w_LightProperties(core::LightParams&) const;
        int w_ListLights(const std::vector<core::SceneLight>& lights) const;

        std::optional<std::string> w_AddModel() const;

    private:
        ImGuiWindowFlags BaseWindowFlags() const noexcept;
        bool Begin(const char *name) const;
        void TextureComboList(int& texture_id, std::string_view caption, const std::vector<std::string>& list_textures, int& item_selected) const;

        GLFWwindow* window_ = nullptr;
        ImGuiIO& io_;
        core::resman::ResourceManager& m_resman;
};

} // namespace avion::widget
