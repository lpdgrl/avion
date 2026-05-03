#pragma once

#include <string>

#include "glad/glad.h"

#include "AvionEngineCore/controller/controller.hpp"

#include "AvionEngineCore/renderer/framebuffer.hpp"

#include "profiler.hpp"
#include "resource_manager.hpp"

namespace avion::gfx
{
  class Pipeline;
}

namespace avion::core {
    class Window {
    public:

        using PressedKeys       = std::array<bool, controller::SIZE_ARRAY_KEYS>;
        using CoordinateOffset  = controller::CoordOffset;
        using ResManager        = resman::ResourceManager;
        using Pipeline          = gfx::Pipeline;

        Window() = delete;
        Window(const std::string& window_name, int width, int height, Pipeline& pipeline, Profiler& profiler);
        Window(const char* window_name, int width, int height, Pipeline& pipeline, Profiler& profiler);

        Window(const Window& other_window) = delete;
        Window(Window&& other_window) = delete;

        Window& operator=(const Window& other_window) = delete;
        Window& operator=(Window&& other_window) = delete;

        // TODO: Add impletation dtor
        ~Window();

        void Init();
        void ProcessEvents();
        void Update();

        bool WindowShouldClose();
        void PollEvents();
        void SwapBuffers();

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

        void DeltaTimeUpdate() noexcept;
        void FramePerSecond() noexcept;

        // TODO: OpenGL funct is dirty arch
        void GlEnable() const noexcept;
        void ClearColorGl(float r, float g, float b) noexcept;
        void GlViewPort(float width, float height) noexcept;


        inline void SetFrameBuffer(gfx::FrameBuffer* buffer)
        {
          m_framebuffer = buffer;
        }

    private:
        void CreateWindow();
        void Render();
        void GetLastPosCursor() noexcept;

    private:
        std::string window_name_;
        int width_window_ = 0;
        int height_window_ = 0;

        controller::Controller controller_;
    
        GLFWwindow* window_ = nullptr;
        Pipeline& m_pipeline;
        Profiler& m_profiler;

        gfx::FrameBuffer* m_framebuffer = nullptr;

        // TODO: Understand how to works it (calculate delay and fps)
        GLfloat delta_time_ = 0.f;
        GLfloat last_time_ = 0.f;

        bool cursor_state_ = true;
    };
} // namespace avion::core
