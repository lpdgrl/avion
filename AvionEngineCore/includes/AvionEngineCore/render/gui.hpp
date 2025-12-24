#pragma once 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    void CustomWindow();

private:
    GLFWwindow* window_ = nullptr;
    ImGuiIO& io_;
};