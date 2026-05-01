#include "AvionEngineEditor/editor/editor_app.hpp"

namespace avion::editor::app
{

  EditorApp::EditorApp(const char* name_window, int width, int height)
  : m_gui_context(m_engine.GetResourceManager())
  , m_editor_gui(detail::EditorContext{.engine = m_engine})
  {
    m_window = std::make_unique<Window>(name_window, width, height, m_engine.GetPipeline(), m_engine.GetProfiler());
    m_window->Init();
    m_engine.Init(width, height);
  }

  bool EditorApp::Init()
  {
    m_gui_context.Init(m_window->GetPointer());

    return true;
  }

  void EditorApp::Run()
  {
    RenderLoop();
  }

  void EditorApp::Exit()
  {

  }

  void EditorApp::RenderLoop()
  {
    while(!m_window->WindowShouldClose())
    {
      m_window->DeltaTimeUpdate();
      m_window->ProcessEvents();
      // TODO: Maybe apply command pattern
      m_window->ClearColorGl();

      m_engine.Render();

      RunFrame();

      m_window->SwapBuffers();
      m_window->PollEvents();
      m_window->FramePerSecond();
    }
  }

  bool EditorApp::RunFrame()
  {
    m_gui_context.BeginFrame();

    m_editor_gui.Render();
    
    m_gui_context.EndFrame();

    return true;
  }

  bool EditorApp::Shutdown()
  {
    m_gui_context.Shutdown();
    m_engine.Shutdown();
  
    return true;
  }

} // namespace avion::editor::app