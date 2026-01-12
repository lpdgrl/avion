#pragma once

#include <string>
#include <iostream>
#include <iomanip>

#include "glad/glad.h"

#include "../controller/controller.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "gui.hpp"

static const int kObjectsCreate = 20;

class Window {
public:
    using PressedKeys = std::array<bool, SIZE_ARRAY_KEYS>;
    using CoordinateOffset = CoordOffset;

    Window() = delete;
    Window(const std::string& window_name, int width, int height);
    Window(const char* window_name, int width, int height);

    Window(const Window& other_window) = delete;
    Window(Window&& other_window) = delete;

    Window& operator=(const Window& other_window) = delete;
    Window& operator=(Window&& other_window) = delete;

    // TODO: Add impletation dtor
    ~Window();

    void Init();
    void ProcessEvents();
    void Update();

    // Callbacks OpenGL
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow*      GetPointer()            const;
    int              GetWidth()              const noexcept;
    int              GetHeight()             const noexcept;
    GLfloat          GetDeltaTime()          const noexcept;
    CoordinateOffset GetOffsetController()   noexcept;
    bool             WasReleasedKey(int key) const noexcept;
    bool             WasPressedKey(int key)  const noexcept;
    bool             IsDown(int key)         const noexcept;

private:
    void CreateWindow();
    void DeltaTimeUpdate() noexcept;
    void Render();
    void FramePerSecond() noexcept;

private:
    std::string window_name_;
    int width_window_ = 0;
    int height_window_ = 0;
    Controller controller_;
    Scene scene_;
    Gui* gui_;
    Renderer* render_;
    GLFWwindow* window_ = nullptr;
    // TODO: Understand how to works it (calculate delay and fps)
    GLfloat delta_time_ = 0.f;
    GLfloat last_time_ = 0.f;
    double lt_ = 0;
    double delay_ = 0;
    int fps_ = 0;
};