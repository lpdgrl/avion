#ifndef AVION_EDITOR_APP_H
#define AVION_EDITOR_APP_H

#include "AvionEngineCore/core/engine.hpp"
#include "AvionEngineCore/core/window.hpp"
#include "AvionEngineCore/api/backend/IRenderApp.hpp"

#include "AvionEngineEditor/ui/av_gui_context.hpp"
#include "AvionEngineEditor/editor/editor_gui.hpp"

namespace avion::editor::app
{
  class EditorApp;

  class RenderEditorApp : public core::common::IRenderApp
  {
    public:
      RenderEditorApp(EditorApp& app);
      ~RenderEditorApp() = default;

      void Render() override;

    private:
      EditorApp& m_app;
  };

  class EditorApp
  {
    public:
      using Engine        = core::engine::Engine;
      using AvGuiContext  = ui::avguicontext::AvGuiContext;
      using EditorGui     = gui::EditorGui;

      EditorApp(const char* name_window, int width, int height);

      bool Init();
      void Run();
      void Exit();

      void RenderLoop();

      ~EditorApp() = default;

    private:
      bool RunFrame();
      bool Shutdown();

    private:
      Engine           m_engine; 
      AvGuiContext     m_gui_context;
      EditorGui        m_editor_gui;
  };
  

} // namespace avion::editor::app


#endif