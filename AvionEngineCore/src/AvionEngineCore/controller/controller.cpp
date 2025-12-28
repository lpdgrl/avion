#include "AvionEngineCore/controller/controller.hpp"

Controller::Controller(double xpos_cursor, double ypos_cursor): last_xpos_cursor_(xpos_cursor), last_ypos_cursor_(ypos_cursor) {}

void Controller::KeyPressed(GLFWwindow* window) {
    for (int i = 0; i < keys_.size(); ++i) {
        keys_[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }
}

const std::array<bool, SIZE_ARRAY_KEYS>& Controller::GetPressKeys() const {
    return keys_;
}

void Controller::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Controller* self = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->OnMouseMove(xpos, ypos);
    }
}

void Controller::OnMouseMove(double xpos, double ypos) {
    // std::cout << "xpos: " << xpos << " ypos: " << ypos << '\n';
    xoffset_ = xpos - last_xpos_cursor_;
    yoffset_ = last_ypos_cursor_ - ypos;

    last_xpos_cursor_ = xpos;
    last_ypos_cursor_ = ypos;
}

CoordOffset Controller::GetOffset() {
    CoordOffset offset{xoffset_, yoffset_};
    xoffset_ = 0;
    yoffset_ = 0;
    return offset;
}

void Controller::SetCoordinate(double xpos_cursor, double ypos_cursor) {
    last_xpos_cursor_ = xpos_cursor;
    last_ypos_cursor_ = ypos_cursor;
}