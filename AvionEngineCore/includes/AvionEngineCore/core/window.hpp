#pragma once

#include <string>

#include "glad/glad.h"

#include "AvionEngineCore/controller/controller.hpp"

#include "AvionEngineCore/core/profiler.hpp"
#include "AvionEngineCore/core/resource_manager.hpp"

#include "AvionEngineCore/core/Common/CameraProxy.hpp"

namespace avion::core::detail
{
  struct WindowSize
  {
    int width{};
    int height{};
  };
} // namespace avion::core::detail

namespace avion::core {
    
    class Window {
    public:
        using PressedKeys       = std::array<bool, controller::SIZE_ARRAY_KEYS>;
        using CoordinateOffset  = controller::CoordOffset;
        using ResManager        = resman::ResourceManager;
        using WindowSize        = core::detail::WindowSize;

        using CameraProxy = core::common::CameraProxy;

        Window() = delete;
        Window(const std::string& window_name, int width, int height, Profiler& profiler, CameraProxy& camera_proxy);
        Window(const char* window_name, int width, int height, Profiler& profiler, CameraProxy& camera_proxy);

        Window(const Window& other_window) = delete;
        Window(Window&& other_window) = delete;

        Window& operator=(const Window& other_window) = delete;
        Window& operator=(Window&& other_window) = delete;

        // TODO: Add impletation dtor
        ~Window();

        void Init();
        void ProcessEvents();
        
        bool WindowShouldClose();
        void PollEvents();
        void SwapBuffers();

        // Callbacks OpenGL
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

        GLFWwindow*      GetPointer()            const;
        WindowSize       GetSize()               const noexcept;
        int              GetWidth()              const noexcept;
        int              GetHeight()             const noexcept;
        GLfloat          GetDeltaTime()          const noexcept;
        CoordinateOffset GetOffsetController()   noexcept;
        bool             WasReleasedKey(int key) const noexcept;
        bool             WasPressedKey(int key)  const noexcept;
        bool             IsDown(int key)         const noexcept;

        void DeltaTimeUpdate() noexcept;
        void FramePerSecond() noexcept;

    private:
        void CreateWindow();
        void Render();
        void GetLastPosCursor() noexcept;

    private:
        enum class MovementKey : int
        {
          kForward = 0,
          kBackward,
          kLeft,
          kRight
        };

        std::string window_name_;
        int width_window_ = 0;
        int height_window_ = 0;

        controller::Controller controller_;
    
        GLFWwindow* window_ = nullptr;
        Profiler& m_profiler;
        CameraProxy& m_camera_proxy;

        // TODO: Understand how to works it (calculate delay and fps)
        GLfloat delta_time_ = 0.f;
        GLfloat last_time_ = 0.f;

        bool cursor_state_ = true;
    };
} // namespace avion::core
