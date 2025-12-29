#include "AvionEngineCore/controller/controller.hpp"

Controller::Controller(double xpos_cursor, double ypos_cursor): last_xpos_cursor_(xpos_cursor), last_ypos_cursor_(ypos_cursor) {}

Controller::~Controller() {
    std::cout << "Controller is destroyed" << '\n';
}

void Controller::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Controller* self = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->OnMouseMove(xpos, ypos);
    }
}

void Controller::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Controller* self = static_cast<Controller*>(glfwGetWindowUserPointer(window));
    if (self) {
        self->OnKeyPress(key, scancode, action, mods);
    }
}

void Controller::OnKeyPress(int key, int scancode, int action, int mods) {
    if (key < 0 || key > GLFW_KEY_LAST) {
        return;
    }

    if (action == GLFW_PRESS) {
        isDown_[key] = true;
        wasPressed_[key] = true;
    } else if (action == GLFW_RELEASE) {
        isDown_[key] = false;
        wasReleased_[key] = true;
    }
}

bool Controller::IsDown(int key) const {
    return isDown_[key];
}

bool Controller::WasPressed(int key) const {
    return wasPressed_[key];
}

bool Controller::WasReleased(int key) const {
    return wasReleased_[key];
}

void Controller::ClearStateKeys() {
    std::fill(wasPressed_.begin(), wasPressed_.end(), false);
    std::fill(wasReleased_.begin(), wasReleased_.end(), false);
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