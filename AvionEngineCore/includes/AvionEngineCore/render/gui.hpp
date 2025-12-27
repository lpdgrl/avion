#pragma once 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

class Gui {
public:
    Gui() = delete;
    Gui(GLFWwindow* window);

    Gui(const Gui& other) = delete;
    Gui(Gui&& other) = delete;

    void Init();
    void Frame();
    void Render();
    void CleanUp();

    void WindowAddObject(glm::vec3& position, glm::vec3& size, glm::vec3& color, bool& state_button) const;
    void WindowLigthColor(glm::vec3& color) const;
private:
    GLFWwindow* window_ = nullptr;
    ImGuiIO& io_;
};