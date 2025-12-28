#pragma once

#include "glfw/glfw3.h"

#include <array>
#include <iostream>

static const unsigned int SIZE_ARRAY_KEYS = 512;

enum class KeyPress {
    kKeyW = 87,
    kKeyS = 83,
    kKeyR = 82,
    kKeyI = 73,
    kKeyK = 75,
    kKeyEnter = 257,
    kKeySpace = 32,
    kKeyQ = 81,
    kNone = -1,
};

struct CoordOffset {
    double xoffset = 0;
    double yoffset = 0;
};

// What is does?
// Класс контроллер отвечает за обработку ввода (мышь, клавиатура, геймпад)
class Controller {
public:
    Controller() = default;

    explicit Controller(double xpos_curosr, double ypos_cursor);

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    void KeyPressed(GLFWwindow* window);

    const std::array<bool, SIZE_ARRAY_KEYS>& GetPressKeys() const;

    void OnMouseMove(double xpos, double ypos);
    
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);

    CoordOffset GetOffset();

    void SetCoordinate(double xpos_cursor, double ypos_cursor);

    ~Controller() = default;
private:
    std::array<bool, SIZE_ARRAY_KEYS> keys_;
    double last_xpos_cursor_ = 0;
    double last_ypos_cursor_ = 0;
    double xoffset_ = 0;
    double yoffset_ = 0;
};