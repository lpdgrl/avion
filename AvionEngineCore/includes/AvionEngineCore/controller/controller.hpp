#pragma once

#include <array>

#include "AvionEngineCore/render/render.hpp"

static const unsigned int SIZE_ARRAY_KEYS = 512;

// What is does?
// Класс контроллер отвечает за обработку ввода (мышь, клавиатура, геймпад)
class Controller {
public:
    Controller() = default;

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;

    void KeyPressed(GLFWwindow* window);

    std::array<bool, SIZE_ARRAY_KEYS> GetPressKeys() const;

    ~Controller() = default;
private:
    std::array<bool, SIZE_ARRAY_KEYS> keys_;
};