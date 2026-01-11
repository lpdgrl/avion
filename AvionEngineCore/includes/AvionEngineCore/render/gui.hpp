#pragma once 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glm/glm.hpp"

#include <iostream>
#include <vector>

struct SceneObject;
enum class TypeObject; 

struct Logs {
    int fps = 0;
    double delay = 0;
};

class Gui {
public:
    
    Gui() = delete;
    Gui(GLFWwindow* window);

    Gui(const Gui& other) = delete;
    Gui(Gui&& other) = delete;

    ~Gui();

    void Init();
    void Frame();
    void Render();
    void CleanUp();

    void WindowAddObject(TypeObject& type, glm::vec3& position, glm::vec3& size, glm::vec3& color, bool& state_button) const;
    void WindowLigthColor(glm::vec3& position, glm::vec3& color) const;
    int WindowListObjects(const std::vector<SceneObject>& objects) const;
    void WindowLogs(Logs logs) const;
private:
    GLFWwindow* window_ = nullptr;
    ImGuiIO& io_;
};