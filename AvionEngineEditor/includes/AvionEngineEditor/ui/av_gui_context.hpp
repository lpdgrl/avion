#ifndef AVION_EDITOR_AV_GUI_CONTEXT_H
#define AVION_EDITOR_AV_GUI_CONTEXT_H

  #include "imgui.h"
  #include "imgui_impl_glfw.h"
  #include "imgui_impl_opengl3.h"

  #include "AvionEngineCore/core/resource_manager.hpp"

  namespace avion::ui::avguicontext
  {
    class AvGuiContext
    {
      public:
        using ResManager = core::resman::ResourceManager;
        AvGuiContext() = delete;
        AvGuiContext(ResManager& resman);

        bool Init(GLFWwindow* window);
        bool BeginFrame();
        bool EndFrame();
        bool Shutdown();
        
        ~AvGuiContext() = default;
        
      private:
        ResManager& m_resman;
        ImGuiIO*    m_io;
    };
  } // namespace avion::ui::avguicontext

#endif