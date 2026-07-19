#include "AvionEngineEditor/editor/editor_app.hpp"

namespace avion::editor::app
{

  EditorApp::EditorApp(const char* name_window, int width, int height)
  : m_gui_context(m_engine.GetResourceManager())
  , m_editor_gui(detail::EditorContext{.engine = m_engine, .state{}, .selection_ctx{}})
  {
    
  }

  bool EditorApp::Init()
  {
    m_engine.Init();
    m_gui_context.Init(m_engine.GetWindow().GetPointer());
    m_engine.AddRenderApp(std::make_unique<RenderEditorApp>(*this));

    // m_engine.AddBackendComand("FrameBuffer", "scene"); 
    m_engine.GetBackend().CreateFrameBuffer("scene", 1020, 700);

    return true;
  }

  void EditorApp::Run()
  {
    m_engine.Run();
  }

  void EditorApp::Exit()
  {
    AV_LOG_TODO("EditorApp::Exit(): TO DO NOTHIG");
  }

  void EditorApp::RenderLoop()
  {
    auto& backend = m_engine.GetBackend(); 
    auto& scene_renderer = m_engine.GetSceneRenderer();

    auto& fb_scene = backend.GetFrameBuffer("scene");

    unsigned int w = m_editor_gui.GetContext().state.viewport_width;
    unsigned int h = m_editor_gui.GetContext().state.viewport_height;

    // if (w > 0 && h > 0 && (fb_scene.GetWidth() != w || fb_scene.GetHeight() != h))
    // {
    //   fb_scene.RescaleFrameBuffer(w, h);
    // }
    fb_scene.Bind();
    backend.SetViewportState({0, 0, fb_scene.GetWidth(), fb_scene.GetHeight()});
    backend.SetColorState({0.1f, 0.1f, 0.5f, 1.f});
    backend.SetBlendingState(
      {
        .enabled = true, 
        .blend_source_factor = api::backend::detail::BlendingFunc::SourceAlpha,
        .blend_destination_factor = api::backend::detail::BlendingFunc::OneMinusSourceAlpha
        });
    
    backend.BeginFrame();
    scene_renderer.PrepareRenderItems();
    backend.EndFrame();    
    fb_scene.Unbind();
    backend.ApplyDefaultRenderState();

    m_editor_gui.GetContext().state.texture_id_fbo = fb_scene.GetTexture();
    RunFrame();
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

  RenderEditorApp::RenderEditorApp(EditorApp& app)
  : m_app(app) 
  {

  }

  void RenderEditorApp::Render() 
  {
    m_app.RenderLoop();
  }

} // namespace avion::editor::app