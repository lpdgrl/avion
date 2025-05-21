#include "AvionEngineCore/controller/controller.hpp"

void Controller::KeyPressed(GLFWwindow* window) {
    for (int i = 0; i < keys_.size(); ++i) {
        keys_[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }
}

std::array<bool, SIZE_ARRAY_KEYS> Controller::GetPressKeys() const {
    return keys_;
}