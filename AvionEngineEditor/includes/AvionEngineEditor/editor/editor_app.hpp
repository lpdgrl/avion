#ifndef AVION_EDITOR_EDITOR_APP_H
#define AVION_EDITOR_EDITOR_APP_H

#include "AvionEngineCore/core/engine.hpp"
#include "AvionEngineCore/core/window.hpp"

#include "AvionEngineEditor/ui/av_gui_context.hpp"
#include "editor_gui.hpp"

namespace avion::editor::app
{
  class EditorApp
  {
    public:
      using Engine        = core::engine::Engine;
      using Window        = core::Window;
      using UPtrWindow    = std::unique_ptr<Window>;
      using AvGuiContext  = ui::avguicontext::AvGuiContext;
      using EditorGui     = gui::EditorGui;

      EditorApp(const char* name_window, int width, int height);

      bool Init();
      void Run();
      void Exit();

      ~EditorApp() = default;

    private:
      void RenderLoop();
      bool RunFrame();
      bool Shutdown();

    private:
      Engine        m_engine; 
      UPtrWindow    m_window;
      AvGuiContext  m_gui_context;
      EditorGui     m_editor_gui;
  };

} // namespace avion::editor::app


#endif