#include "AvionEngineEditor/panels/inspector_panel.hpp"

namespace avion::editor::panel
{

  InspectorPanel::InspectorPanel(InspectorPanel::EditorContext context)
  : m_context(context)
  {

  }

  void InspectorPanel::Render() 
  {

    ImVec2 pos(1320, 0);
    ImVec2 size(600, 1080);
    ui::utils::SetupWindow(pos, size, ImGuiCond_Always);

    ScopeWindow window("Inspector", nullptr, ImGuiWindowFlags_NoMove);
    
    if (!window.IsOpen())
    {
      return;
    }
  }

} // namespace avion::editor::panel