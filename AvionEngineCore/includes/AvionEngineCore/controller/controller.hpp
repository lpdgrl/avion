#pragma once

#include "glfw/glfw3.h"

#include <array>
#include <iostream>

namespace avion::controller {

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

    struct CoordMouse {
        double xpos = 0;
        double ypos = 0;
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

        bool IsDown(int key) const;
        bool WasPressed(int key) const;
        bool WasReleased(int key) const;

        void OnMouseMove(double xpos, double ypos);
        void OnKeyPress(int key, int scancode, int action, int mods);

        static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        CoordOffset GetOffset();

        double GetLastXposCursor() const noexcept;
        double GetLastYposCursor() const noexcept;

        void SetCoordinate(double xpos_cursor, double ypos_cursor);

        void ClearStateKeys();

        ~Controller();
    private:
        std::array<bool, SIZE_ARRAY_KEYS> isDown_;
        std::array<bool, SIZE_ARRAY_KEYS> wasPressed_;
        std::array<bool, SIZE_ARRAY_KEYS> wasReleased_;

        double last_xpos_cursor_ = 0;
        double last_ypos_cursor_ = 0;
        double xoffset_ = 0;
        double yoffset_ = 0;
    };

} // namespace avion::controller