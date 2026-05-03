#include "AvionEngineEditor/panels/scene_panel.hpp"

namespace avion::editor::panel
{

  ScenePanel::ScenePanel(ScenePanel::EditorContext& context)
  : m_context(context)
  {

  }

  void ScenePanel::Render() 
  {
    ImVec2 pos(300, 0);
    ImVec2 size(1020, 700);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Scene", nullptr, ImGuiWindowFlags_NoMove);

    if (!window.IsOpen())
    {
      return;
    }
    
    ChildWindow child_window("render");

    ImVec2 size_child = ImGui::GetContentRegionAvail();
    m_context.state.viewport_width  = size_child.x;
    m_context.state.viewport_height = size_child.y;

    ImGui::Image(
      reinterpret_cast<void*>(static_cast<intptr_t>(m_context.state.texture_id_fbo)),
      size_child,
      ImVec2(0, 0),
      ImVec2(1, 1));
    
  }

} // namespace avion::editor::panel