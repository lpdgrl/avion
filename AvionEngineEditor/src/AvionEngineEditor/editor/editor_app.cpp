#include "AvionEngineEditor/editor/editor_app.hpp"

namespace avion::editor::app
{

  EditorApp::EditorApp(const char* name_window, int width, int height)
  : m_gui_context(m_engine.GetResourceManager())
  , m_editor_gui(detail::EditorContext{.engine = m_engine, .state{1, 1, 1}})
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
    m_engine.CreateFrameBuffer(1020, 700);
    // m_window->SetFrameBuffer(&m_engine.GetFrameBuffer());
    RenderLoop();
  }

  void EditorApp::Exit()
  {

  }

  void EditorApp::RenderLoop()
  {
    auto& scene_fbo = m_engine.GetFrameBuffer();
    
    while(!m_window->WindowShouldClose())
    {
      m_window->DeltaTimeUpdate();
      m_window->ProcessEvents();

      unsigned int w = m_editor_gui.GetContext().state.viewport_width;
      unsigned int h = m_editor_gui.GetContext().state.viewport_height;

      if (w > 0 && h > 0 && (scene_fbo.Width() != w || scene_fbo.Height() != h))
      {
          scene_fbo.RescaleFrameBuffer(w, h);
      }

      m_window->ClearColorGl(0.f, 0.f, 0.f);

      m_window->GlViewPort(scene_fbo.Width(), scene_fbo.Height());
      scene_fbo.Bind();

      m_window->ClearColorGl(0.2f, 0.2f, 0.2f);
      m_engine.Render();

      scene_fbo.Unbind();

      m_window->GlViewPort(m_window->GetWidth(), m_window->GetHeight());

      m_editor_gui.GetContext().state.texture_id_fbo = scene_fbo.GetFrameTextures();
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